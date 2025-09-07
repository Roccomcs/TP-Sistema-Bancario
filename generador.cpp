#include <iostream>
#include <cstring>

using namespace std;

// **Generador de transacciones**

//! STRUCTS
struct Cliente{
    char nombre[50];
    int dni;
    char username[50];
    int clave;
    double saldo = 10000;
};

struct Transaccion{
    double monto;
    int fecha;
    int id;
    Cliente per;
    bool ingreso;
};

//! FUNCIONES COMPLEMENTARIAS
Cliente buscarCliente();
void ingreso(Cliente &clie);
void guardarTransaccion(Transaccion trans);

//! FUNCIONES PRINCIPALES
void realizarTransaccion();
void eliminarTransaccion(int id);

int main() {
        
    int opcion=0, idTransaccion=0;

    cout << "------ Menú del Generador de transacciones ------" << endl;

    while(1){
        
        cout << "\n1. Realizar transaccion." << endl;
        cout << "2. Eliminar transaccion." << endl;
        cout << "3. Salir.\n" << endl;
        
        cout << "\nIngrese una opcion: ";
        cin >> opcion; 

        switch(opcion){
            case 1:
                realizarTransaccion();  
                break;
            case 2:
                cout << "Ingresar ID de la transaccion a eliminar: ";
                cin >> idTransaccion;

                eliminarTransaccion(idTransaccion);

                cout << "Transaccion eliminada correctamente." << endl;
                break;
            case 3:
                cout << "Saliendo...";
                return 0;
        }
    }
}

//! FUNCIONES COMPLEMENTARIAS

Cliente buscarCliente(){
    Cliente Buscado;
    cout << "Ingrese su username: " << endl;
    cin>>Buscado.username;
    cout << "Ingrese su clave: " << endl;
    cin>>Buscado.clave;
    FILE *archivo = fopen("clientes.dat", "rb");
    if (archivo != NULL) {
        Cliente cliente;
        while (fread(&cliente, sizeof(Cliente), 1, archivo)) {
            if( (strcmp(Buscado.username,cliente.username) == 0) && (Buscado.clave == cliente.clave) ){
                fclose(archivo);
                return cliente;
            }
        }
        cout<<"Cliente no encontrado, intente de nuevo" << endl;
        fclose(archivo);
        return buscarCliente();
    }
    else{
        cout<< "No se encontro el archivo" << endl;
    }
    
}

//! FUNCIONES PRINCIPALES
void RealizarTransaccion(){
    Cliente cli = buscarCliente();
    cout<<"Bienvenido " << cli.nombre <<" Su saldo actual es de: " << cli.saldo << endl;
    ingreso(cli);
    FILE* cliente = fopen("clientes.dat","rb+");
    if(cliente==NULL)
    {
    	cout<<"No se pudo abrir el archivo" << endl;
    }
    else{
        int pos=0;
    	bool encontrado=false;
    	Cliente clie;
    	while(fread(&clie,sizeof(Cliente),1,cliente)==1 && !encontrado){
    	    pos++;
    		if(cli.dni==clie.dni)
    		{
    			encontrado=true;
    		}
        }
    		fseek(cliente, (pos-1)*sizeof(Cliente), SEEK_SET);
	        fwrite(&cli,sizeof(Cliente),1,cliente);
    }
	fclose(cliente);	
}

void ingreso(Cliente &clie){
    Transaccion trans;
    cout<<"Si desea ingresar dinero ingrese 1, si desea retirar ingrese 0 " << endl;
    trans.per=clie;
    cin>>trans.ingreso;
    if(trans.ingreso==1)
    {
        cout<<"Ingrese el monto que desea ingresar" << endl;
        cin>>trans.monto;
        cout<< "Ingrese la fecha actual en formato aaaa/mm/dd" << endl;
        cin>>trans.fecha;
        clie.saldo += trans.monto;
        cout << "Su nuevo saldo es de: " << clie.saldo << endl;
        guardarTransaccion(trans);
        cout<<"Para realizar una nueva transaccion ingrese 1, de lo contrario ingrese otro numero" << endl;
        int temp;
        cin>>temp;
        if(temp==1){
            ingreso(clie);
        }
        
    }
    if(trans.ingreso==0)
    {
        cout<<"Ingrese el monto que desea retirar" << endl;
        cin>>trans.monto;
        cout<< "Ingrese la fecha actual en formato aaaa/mm/dd" << endl;
        cin>>trans.fecha;
        if(clie.saldo-trans.monto>=0){
            clie.saldo -= trans.monto;
            cout << "Su nuevo saldo es de: " << clie.saldo << endl;
            guardarTransaccion(trans);
        }
        else{
            cout<< "No hay saldo suficiente" << endl;
            ingreso(clie);
        }
        cout<<"Para realizar una nueva transaccion ingrese 1, de lo contrario ingrese otro numero" <<endl;
        int temp;
        cin>>temp;
        if(temp==1){
        ingreso(clie);
        }
    }
    if(trans.ingreso!=0 && trans.ingreso !=1)
    {
        cout<<"Hubo un problema, intente de nuevo" << endl;
        ingreso(clie);
    }
}

void guardarTransaccion(Transaccion trans){
    FILE* transacciones = fopen("transacciones.dat","ab+");
    fseek(transacciones, 0, SEEK_SET);
    
    Transaccion transBuscada;
    int pos=0;
    trans.id=1;   
        while(fread(&transBuscada,sizeof(Transaccion),1,transacciones)==1)
        {
        trans.id=transBuscada.id+1;
         if(trans.fecha>transBuscada.fecha)
         {
         	pos++;
         }
        }
    fwrite(&trans, sizeof(Transaccion),1, transacciones);
    fclose(transacciones);
}

void eliminarTransaccion(int id){
	FILE* temp = fopen("temporal.dat","wb+");
	FILE* transaccion = fopen("transacciones.dat","ab+");
	fseek(transaccion,0,SEEK_SET);
	Transaccion trans;
	while(fread(&trans,sizeof(Transaccion),1,transaccion)==1)
	{
		if(trans.id!=id)
		{
		 	fwrite(&trans,sizeof(Transaccion),1,temp);
		}
	}
	fclose(transaccion);
	fclose(temp);
	FILE* transacciones = fopen("transacciones.dat","wb+");
	FILE* tempp = fopen("temporal.dat","rb");
	while(fread(&trans,sizeof(Transaccion),1,tempp)==1)
	{	
		 	fwrite(&trans,sizeof(Transaccion),1,transacciones);
	}
	fclose(transacciones);
	fclose(tempp);

    remove("temporal.dat");
}
