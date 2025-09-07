#include <iostream>
#include <cstring>


using namespace std;

// **Analista de transacciones**

//! STRUCTS
struct Cliente
{
    char nombre[50];
    int dni;
    char username [50];
    int clave;
    double saldo=10000;
};

struct Transaccion
{
    double monto;
    int fecha;
    int id;
    Cliente per;
    bool ingreso; 
};

//! FUNCIONES COMPLEMENTARIAS
string ingreso_egreso(bool ingreso);
void reemplazarArrayChar(char array1[], char array2[]);
int contarTransaccionesTotal();
int contarClientesTotal();
int contarTransaccionesCliente(int dniBuscado);
int calcularPaginas(int cantTransacciones);
void deArchivoToArray(int dniCliente, Transaccion transCliente[]);
void ordenarArrayPorFecha(Transaccion trans[], int &len);
int buscarDNI (Transaccion ArrCli[], int len, int valor);
int restar30Dias(int fecha);
void ordenarArchivo();

//! FUNCIONES PRINCIPALES
void listarTransaccionesCliente(int dniBuscado);
void listarCantidadTransacciones(int dniBuscado);
void montoMax();
void clienteMasIngresos();

int main(){
    
    int opcion=0, dniCliente=0;

    cout << "------ Menú del Analista de transacciones ------" << endl;

    while(1){
        
        cout << "\n1. Listar transacciones de un cliente especifico." << endl;
        cout << "2. Resumen de de las transacciones de los ultimos 30 dias de un cliente especifico." << endl;
        cout << "3. Listar transaccion con el mayor monto." << endl;
        cout << "4. Listar el cliente que tuvo mas ingresos en los ultimos 30 dias." << endl;
        cout << "5. Salir.\n" << endl;
        
        cout << "\nIngrese una opcion: ";
        cin >> opcion; 

        switch(opcion){
            case 1:
                cout << "\nIngrese el DNI del cliente: " << endl;
                cin >> dniCliente;

                listarTransaccionesCliente(dniCliente);
                break;
            case 2:
                cout << "\nIngrese el DNI del cliente: " << endl;
                cin >> dniCliente;

                listarCantidadTransacciones(dniCliente);
                break;
            case 3:
                montoMax();
                break;
            case 4:
                clienteMasIngresos();
                break;
            case 5:
                cout << "Saliendo...";
                return 0;
        }
    }
}

//! FUNCIONES COMPLEMENTARIAS
string ingreso_egreso(bool ingreso){ //~ DEVUELVE STRING SEGUN SI ES INGRESO O EGRESO LA TRANSACCION.
    if(ingreso)
    {
        return "Ingreso";
    }
    return "Egreso";
}

int contarTransaccionesTotal(){ //~ CUENTA LA TOTALIDAD DE LAS TRANSACCIONES EN EL ARCHIVO transacciones.dat
    
    int contadorTrans=0;
    FILE* archivo = fopen("transacciones.dat", "rb");
    if( archivo != NULL){
        Transaccion trans;
        while( (fread(&trans, sizeof(Transaccion), 1, archivo)) == 1){
            contadorTrans++;
        }
    }
    else{
        cout << "No se pudo abrir el archivo para lectura." << endl;
    }
    fclose(archivo);
    return contadorTrans;

}

int contarClientesTotal(){ //~ CUENTA LA TOTALIDAD DE CLIENTES EN EL ARCHIVO transacciones.dat
    
    int contadorCli=0;
    FILE* archivo = fopen("clientes.dat", "rb");
    if( archivo != NULL){
        Cliente Cli;
        while( (fread(&Cli, sizeof(Cliente), 1, archivo)) == 1){
            contadorCli++;
        }
    }
    else{
        cout << "No se pudo abrir el archivo para lectura." << endl;
    }
    fclose(archivo);
    return contadorCli;

}

int contarTransaccionesCliente(int dniBuscado){ //~ CUENTA LA CANTIDAD DE TRANSACCIONES DE UN SOLO CLIENTE DEL ARCHIVO transacciones.dat
    
    int contadorTransCliente=0;
    FILE* archivo = fopen("transacciones.dat", "rb");
    if( archivo != NULL){
        Transaccion trans;
        while( (fread(&trans, sizeof(Transaccion), 1, archivo)) == 1){
            if(dniBuscado == trans.per.dni){
                contadorTransCliente++;
            }
        }
    }
    else{
        cout << "No se pudo abrir el archivo para lectura." << endl;
    }
    fclose(archivo);
    return contadorTransCliente;

}

int calcularPaginas(int cantTransacciones){ //~ CALCULA CUANTAS PAGINAS TENDRA QUE USAR LA FUNCION listarTransCliente().

    if( cantTransacciones % 5 == 0){
        return cantTransacciones/5;
    }
    else{
        return (cantTransacciones/5) + 1;
    }

}

void deArchivoToArray(int dniCliente, Transaccion transCliente[] ){ //~ PASA LAS TRANSFERENCIAS DE UN CLIENTE DEL ARCHIVO A UN ARRAY DE LONGITUD AUTOMATICA (MIDE CUANTAS TRANSFERENCIAS TIENE EL CLIENTE).

    int cantTransCliente = contarTransaccionesCliente(dniCliente);
    FILE* archivo = fopen("transacciones.dat", "rb");
    if(archivo != NULL){
        for(int i=0; i < cantTransCliente; i++) {
            Transaccion trans;
            if( fread(&trans, sizeof(Transaccion), 1, archivo) == 1){
                if(dniCliente == trans.per.dni){
                    transCliente[i].per.dni = trans.per.dni;
                    strcpy(transCliente[i].per.username, trans.per.username);
                    transCliente[i].per.clave = trans.per.clave;

                    transCliente[i].id = trans.id;
                    transCliente[i].ingreso = trans.ingreso;
                    transCliente[i].monto = trans.monto;
                    transCliente[i].fecha = trans.fecha;
                }
                else{
                    i--;
                }
            }
        }
    }
    else{
        cout << "No se pudo abrir el archivo para lectura." << endl;
    }
    fclose(archivo);

}

void ordenarArrayPorFecha(Transaccion trans[], int &len){ //~ ORDENA EL ARRAY CON LAS TRANSACCIONES DEL CLIENTE POR FECHA.
        for (int i=0; i<len-1; i++){
            for(int j=0; j<len-i-1; j++){
                if(trans[j].fecha >= trans[j+1].fecha){
                    Transaccion temp = trans[j];
                    trans[j] = trans[j+1];
                    trans[j+1] = temp;
                }
            }
        }
}

int buscarDNI (Transaccion ArrCli[], int len, int valor){ //~ BUSCA EL DNI DE LA PERSONA, Y SI LO ENCUENTRA DEVUELVE LA POSICION.
 int i = 0;
    while (i < len && ArrCli[i].per.dni != valor) {
         i++;
    }
    if (i == len) {
        return -1;
    }  
    else {
        return i;
    }
} 

int restar30Dias(int fecha){ //~ RESTA 30 DIAS A LA FECHA ESPECIFICADA.
    int anio = fecha / 10000;
    int mes = (fecha / 100) % 100;
    int dia = fecha % 100;

    int diasEnMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    dia -= 30;

    while (dia <= 0) {
        mes--;
        if (mes < 1) {
            mes = 12;
            anio--;
        }
        dia += diasEnMes[mes - 1];
    }

    return anio * 10000 + mes * 100 + dia;
}

void ordenarArchivo(){ //~ ORDENA EL ARCHIVO transacciones.dat
    Transaccion temp;
    int cantTrasaccionesTotal = contarTransaccionesTotal();
    Transaccion arrayAux[cantTrasaccionesTotal];
    int lenAux=0;

    FILE* archivo = fopen("transacciones.dat", "rb");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo\n");
        return;
    }

    while (fread(&temp, sizeof(Transaccion), 1, archivo) == 1) {
        arrayAux[lenAux] = temp;
        lenAux++;
    }
    fclose(archivo);

    ordenarArrayPorFecha(arrayAux, lenAux);

    archivo = fopen("transacciones.dat", "wb");
    for (int i = 0; i < lenAux; i++) {
        fwrite(&arrayAux[i], sizeof(Transaccion), 1, archivo); 

    }
    fclose(archivo);
}

//! FUNCIONES PRINCIPALES
void listarTransaccionesCliente(int dniBuscado){ //~ FUNCION PUNTO 1 DEL ANALISTA DE TRANSACCIONES (pasando a array)

    int lenAux = contarTransaccionesCliente(dniBuscado);

    if(lenAux == 0){
        cout << "\nNo se encontraron transacciones del cliente." << endl;
        return;
    }

    Transaccion transacciones[contarTransaccionesCliente(dniBuscado)];

    deArchivoToArray(dniBuscado, transacciones);

    ordenarArrayPorFecha(transacciones, lenAux);
    
    int cantTransCliente = contarTransaccionesCliente(dniBuscado);
    int cantPaginas = calcularPaginas(cantTransCliente);  
    int contPaginas=0;

    int inicio=0;
    int final=5;
    int aux=0;

    char charNavegacion;

    cout << "Transacciones del DNI N° " << dniBuscado  << ".\n" << endl;
    
    while(contPaginas <= cantPaginas){
        
        contPaginas++;
        if(contPaginas == cantPaginas){
            final = cantTransCliente;
        }

        for(int i=inicio; i<final; i++){
            
            cout << "--------------------------" << endl;
            cout << "Datos del la transaccion " << i+1 << "." << endl;
            cout << "ID: " << transacciones[i].id << endl;
            cout << "Fecha: " << transacciones[i].fecha << endl;
            cout << "Tipo: " << ingreso_egreso(transacciones[i].ingreso) << endl;
            cout << "Monto: " << transacciones[i].monto << endl;
            cout << "--------------------------" << "\n" << endl;

            aux = i;

        }
        
        if( (contPaginas == 1) && (contPaginas < cantPaginas) ){
            cout << "S - Pág. siguiente / E - Salir: ";
        }
        else if( (contPaginas == 1) && (contPaginas == cantPaginas) ){
            cout << "E - Salir: ";
        }
        else if( (contPaginas == cantPaginas) && (contPaginas != 1) ){
            cout << "A - Pág. anterior / E - Salir: ";
        }
        else if( (contPaginas > 1) && (contPaginas < cantPaginas) ){
            cout << "S - Pág. siguiente / A - Pág. anterior / E - Salir: ";
        }
        
        cin >> charNavegacion;

        if((contPaginas >= 1) && (contPaginas < cantPaginas) && (charNavegacion == 'S' || charNavegacion == 's')){
            inicio = aux + 1;
            final = inicio + 5;
            continue;
        }
        else if((contPaginas > 1) && (contPaginas <= cantPaginas) && (charNavegacion == 'A' || charNavegacion == 'a')){
            inicio = inicio - 5;
            final = inicio + 5;
            contPaginas = contPaginas - 2;
        }
        else if((contPaginas >= 1) && (contPaginas <= cantPaginas) && (charNavegacion == 'E' || charNavegacion == 'e')){
            cout << "Saliendo..." << endl;
            contPaginas = cantPaginas + 1;
        }
    }
}

void listarCantidadTransacciones(int dniBuscado){ //~ FUNCION PUNTO 2 DEL ANALISTA DE TRANSACCIONES
    
    int cantIngresos=0, cantEgresos=0, fechaActual=0;
    double sumaIngresos=0, sumaEgresos=0;

    FILE* archivo = fopen("transacciones.dat", "rb");
    if( archivo != NULL){

        cout << "Ingresar fecha de hoy (AAAAMMDD): "; 
        cin >> fechaActual;
        int fechaDesde = restar30Dias(fechaActual); ;

        Transaccion trans;
        while(fread(&trans, sizeof(Transaccion), 1, archivo) == 1){
            if( dniBuscado == trans.per.dni &&  fechaDesde <= fechaActual ){
                if(trans.ingreso == 1){
                    cantIngresos = cantIngresos + 1;
                    sumaIngresos = sumaIngresos + trans.monto;
                }
                else if(trans.ingreso == 0){
                    cantEgresos = cantEgresos + 1;
                    sumaEgresos = sumaEgresos + trans.monto;
                }
            }
        }

        cout << "Resumen de transacciones del usuario con DNI N° " << dniBuscado << "." << endl;
        cout << "Hubo un total de " << cantIngresos << " ingreso/s, sumando un total de $" << sumaIngresos << "." << endl;
        cout << "Hubo un total de " << cantEgresos << " egreso/s, sumando un total de $" << sumaEgresos << "." << endl;

        fclose(archivo);
    }
    else{
        cout << "No se pudo abrir el archivo para lectura." << endl;
    }
    
}

void montoMax(){ //~ FUNCION PUNTO 3 DEL ANALISTA DE TRANSACCIONES

    int idMax = contarTransaccionesTotal();
    
    Transaccion transMax;
    transMax.monto=0;

    char username[50];
    int fechaMax=0;
    bool ingresoMax;
    
    Transaccion trans;
    FILE* archivo = fopen("transacciones.dat", "rb");

    for(int i = 0 ; i < idMax; i++){
        fread(&trans, sizeof(Transaccion), 1, archivo);
        if(trans.monto > transMax.monto){
            transMax = trans;
        }
    }

    cout << "----------------------------" << endl;
    cout << "La transaccion con el monto más alto fue: " << endl;
    cout << "USER: " << transMax.per.username << endl;
    cout << "FECHA: " << transMax.fecha << endl;
    cout << "TIPO: " << ingreso_egreso(transMax.ingreso) << endl;
    cout << "MONTO MÁXIMO: " << transMax.monto << endl;
    cout << "----------------------------" << endl;
    
    fclose(archivo);

}

void clienteMasIngresos(){ //~ FUNCION PUNTO 4 DEL ANALISTA DE TRANSACCIONES
    Transaccion trans;
    int const CantCli = contarClientesTotal();
    Transaccion CliTrans[CantCli];

    FILE* archivo = fopen("transacciones.dat", "rb");
    ordenarArchivo();
    fread(&trans, sizeof(Transaccion), 1, archivo);

    int FechaActual = trans.fecha;
    int FechaDesde = restar30Dias(FechaActual); 

    int IdArray = 0;

    fseek(archivo, 0 , SEEK_SET);
    while(fread(&trans , sizeof(Transaccion), 1, archivo)){
        if(trans.fecha >= FechaDesde){
            if(trans.ingreso == 1 ){
                int Pos = buscarDNI(CliTrans , CantCli , trans.per.dni);
                if(Pos >= 0){
                    CliTrans[Pos].monto += trans.monto;
                }
                else{
                    CliTrans[IdArray] = trans;
                    IdArray++ ; 
                }
            }
        }
        else{
            break;
        }
    } 
    fclose(archivo);   
    
    double Ingresos=0;
    char Username[50];
    int FechaMax=0;
    
    for(int i = 0 ; i < IdArray; i++){
        if(CliTrans[i].monto > Ingresos){
            Ingresos = CliTrans[i].monto;
            strcpy(Username, CliTrans[i].per.username);
            FechaMax = CliTrans[i].fecha;
        }
    }
    
    cout << "----------------------------------" << endl;
    cout << "El cliente con más ingresos fue: " << endl;
    cout << "USER: " << Username << endl;
    cout << "INGRESOS: " << Ingresos << endl;
    cout << "----------------------------------" << endl;
}
