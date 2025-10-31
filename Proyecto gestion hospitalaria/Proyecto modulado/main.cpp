#include <iostream>
#include "estructuras.h"
#include "funciones.h"
#include "menus.h"

using namespace std;

int main()
{
	cout << "INICIANDO SISTEMA DE GESTIÓN HOSPITALARIA" << endl;
    cout << "=============================================" << endl;
    
    Hospital* hospital = inicializarHospital(
        "Hospital Central", 
        "Av. Principal #123", 
        
    );
    
    menuPrincipal(hospital);
    
    destruirHospital(hospital);
    
    return 0;
}
