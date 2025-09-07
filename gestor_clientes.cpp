#include <iostream>
#include <cstring>

using namespace std;

// **Gestor de clientes**

//! STRUCTS
struct Cliente{
    char nombre[50];
    int dni;
    char username[50];
    int clave;
    double saldo=10000;
};

struct Transaccion{
    double monto;
    int fecha;
    int id;
    Cliente per;
    bool ingreso;
};

//! FUNCIONES COMPLEMENTARIAS
bool repiteDNI(int dniBuscado);
bool repiteUsername(char usernameBuscado[]);

//! FUNCIONES PRINCIPALES
void registrarClientes();

int main() {
    registrarClientes();
    return 0;
}

//! FUNCIONES COMPLEMENTARIAS

bool repiteDNI(int dniBuscado) {
    FILE *archivo = fopen("clientes.dat", "rb");
    if (archivo != NULL) {
        Cliente cliente;
        while (fread(&cliente, sizeof(Cliente), 1, archivo) == 1) {
        	
            if (cliente.dni == dniBuscado) {
            	
                fclose(archivo);
                return true;
            }
            
        }
        fclose(archivo);
        return false;
    } else {
        cout << "No se pudo abrir el archivo para lectura." <<endl;
        return false;
    }
    return false;
}

bool repiteUsername(char usernameBuscado[]) {
    FILE *archivo = fopen("clientes.dat", "rb");
    if (archivo != NULL){
        Cliente cliente;
        while (fread(&cliente, sizeof(Cliente), 1, archivo) == 1) {
        	    if(strcmp(usernameBuscado,cliente.username) == 0){
                fclose(archivo);
                return true;
               }
            
        }
        fclose(archivo);
        return false;
    } else {
        cout << "No se pudo abrir el archivo para lectura." <<endl;
        return false;
    }
    return 0;
}

//! FUNCIONES PRINCIPALES
void registrarClientes(){
    
    Cliente clienteNuevo[1];
    FILE *clientes = fopen("clientes.dat", "ab");
    cout<<" Registrar a un cliente nuevo"<<endl;
    cout<< "Ingresar su nombre"<<endl;
    cin.getline(clienteNuevo[0].nombre,50);
    cout<<"Ingrese DNI sin puntos ni comas"<<endl;
    cin>>clienteNuevo[0].dni;

    while(repiteDNI(clienteNuevo[0].dni)==true) {
        cout << "Ya existe el DNI, ingresar otro" << endl;
        cin>> clienteNuevo[0].dni;
    }
    cout<<"Ingrese username que no contenga espacios ni caracteres especiales"<<endl;  
    cin>> clienteNuevo[0].username;

    while (repiteUsername(clienteNuevo[0].username)==true){
        cout << "Ya existe este username"<<endl;
        cin>> clienteNuevo[0].username;
    }

    cout<<"Ingrese clave numerica: " <<endl;
    cin>>clienteNuevo[0].clave;

    fwrite(&clienteNuevo, sizeof(Cliente), 1, clientes);
    fclose(clientes);

    cout << "Cliente registrado, saldo inicial : $" << clienteNuevo[0].saldo << endl;
}