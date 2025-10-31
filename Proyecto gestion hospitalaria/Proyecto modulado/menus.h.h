#ifndef MENUS_H
#define MENUS_H

#include <iostream>
#include "FUNCIONES.h"

using namespace std;




// ============================================================================
// INTERFAZ DE USUARIO - MENÚS
// ============================================================================

void menuPacientes(Hospital* hospital) {
    int opcion;
    do {
        cout << "\n+----------------------------------------+" << endl;
        cout << "¦          GESTIÓN DE PACIENTES         ¦" << endl;
        cout << "¦----------------------------------------¦" << endl;
        cout << "¦ 1. Registrar nuevo paciente           ¦" << endl;
        cout << "¦ 2. Buscar paciente por cédula         ¦" << endl;
        cout << "¦ 3. Ver historial médico               ¦" << endl;
        cout << "¦ 4. Listar todos los pacientes         ¦" << endl;
        cout << "¦ 0. Volver al menú principal           ¦" << endl;
        cout << "+----------------------------------------+" << endl;
        cout << "Opción: ";
        cin >> opcion;
        limpiarBuffer();
        
        switch (opcion) {
            case 1:
                crearPaciente(hospital);
                break;
            case 2: {
                char cedula[20];
                cout << "Cédula a buscar: ";
                cin.getline(cedula, 20);
                Paciente* paciente = buscarPacientePorCedula(hospital, cedula);
                if (paciente != nullptr) {
                    cout << "Paciente encontrado: " << paciente->nombre << " " << paciente->apellido << endl;
                    cout << "Teléfono: " << paciente->telefono << endl;
                    cout << "Email: " << paciente->email << endl;
                } else {
                    cout << "Paciente no encontrado." << endl;
                }
                break;
            }
            case 3: {
                int id;
                cout << "ID del paciente: ";
                cin >> id;
                limpiarBuffer();
                Paciente* paciente = buscarPacientePorId(hospital, id);
                if (paciente != nullptr) {
                    mostrarHistorialMedico(paciente);
                } else {
                    cout << "Paciente no encontrado." << endl;
                }
                break;
            }
            case 4:
                listarPacientes(hospital);
                break;
            case 0:
                cout << "Volviendo al menú principal..." << endl;
                break;
            default:
                cout << "Opción inválida." << endl;
        }
    } while (opcion != 0);
}

void menuDoctores(Hospital* hospital) {
    int opcion;
    do {
        cout << "\n+----------------------------------------+" << endl;
        cout << "¦           GESTIÓN DE DOCTORES          ¦" << endl;
        cout << "¦----------------------------------------¦" << endl;
        cout << "¦ 1. Registrar nuevo doctor             ¦" << endl;
        cout << "¦ 2. Listar todos los doctores          ¦" << endl;
        cout << "¦ 0. Volver al menú principal           ¦" << endl;
        cout << "+----------------------------------------+" << endl;
        cout << "Opción: ";
        cin >> opcion;
        limpiarBuffer();
        
        switch (opcion) {
            case 1:
                crearDoctor(hospital);
                break;
            case 2:
                listarDoctores(hospital);
                break;
            case 0:
                cout << "Volviendo al menú principal..." << endl;
                break;
            default:
                cout << "? Opción inválida." << endl;
        }
    } while (opcion != 0);
}

void menuCitas(Hospital* hospital) {
    int opcion;
    do {
        cout << "\n+----------------------------------------+" << endl;
        cout << "¦            GESTIÓN DE CITAS            ¦" << endl;
        cout << "¦----------------------------------------¦" << endl;
        cout << "¦ 1. Agendar nueva cita                 ¦" << endl;
        cout << "¦ 2. Listar citas agendadas             ¦" << endl;
        cout << "¦ 0. Volver al menú principal           ¦" << endl;
        cout << "+----------------------------------------+" << endl;
        cout << "Opción: ";
        cin >> opcion;
        limpiarBuffer();
        
        switch (opcion) {
            case 1:
                agendarCita(hospital);
                break;
            case 2: {
                cout << "\n=== CITAS AGENDADAS ===" << endl;
                if (hospital->cantidadCitas == 0) {
                    cout << "No hay citas agendadas." << endl;
                } else {
                    int citasActivas = 0;
                    for (int i = 0; i < hospital->cantidadCitas; i++) {
                        if (strcmp(hospital->citas[i].estado, "Agendada") == 0) {
                            Paciente* p = buscarPacientePorId(hospital, hospital->citas[i].idPaciente);
                            Doctor* d = buscarDoctorPorId(hospital, hospital->citas[i].idDoctor);
                            if (p != nullptr && d != nullptr) {
                                cout << "Cita #" << hospital->citas[i].id << " - " 
                                     << hospital->citas[i].fecha << " " << hospital->citas[i].hora 
                                     << " - " << p->nombre << " " << p->apellido 
                                     << " con Dr. " << d->nombre << " " << d->apellido << endl;
                                citasActivas++;
                            }
                        }
                    }
                    if (citasActivas == 0) {
                        cout << "No hay citas activas agendadas." << endl;
                    }
                }
                break;
            }
            case 0:
                cout << "Volviendo al menú principal..." << endl;
                break;
            default:
                cout << "Opción inválida." << endl;
        }
    } while (opcion != 0);
}

void menuPrincipal(Hospital* hospital) {
    int opcion;
    do {
        cout << "\n+----------------------------------------+" << endl;
        cout << "¦   SISTEMA DE GESTIÓN HOSPITALARIA     ¦" << endl;
        cout << "¦----------------------------------------¦" << endl;
        cout << "¦ 1. Gestión de Pacientes               ¦" << endl;
        cout << "¦ 2. Gestión de Doctores                ¦" << endl;
        cout << "¦ 3. Gestión de Citas                   ¦" << endl;
        cout << "¦ 4. Salir                              ¦" << endl;
        cout << "+----------------------------------------+" << endl;
        cout << "Opción: ";
        cin >> opcion;
        limpiarBuffer();
        
        switch (opcion) {
            case 1:
                menuPacientes(hospital);
                break;
            case 2:
                menuDoctores(hospital);
                break;
            case 3:
                menuCitas(hospital);
                break;
            case 4:
                cout << "Saliendo del sistema..." << endl;
                break;
            default:
                cout << "Opción inválida." << endl;
        }
    } while (opcion != 4);
}

#endif//MENUS_H
