#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <iostream>
#include <cstring>
#include <iomanip>
#include "ESTRUCTURAS.h"

using namespace std;


// ============================================================================
// FUNCIONES DE UTILIDAD Y VALIDACIÓN
// ============================================================================

void limpiarBuffer() {
    cin.clear();
    cin.ignore(10000, '\n');
}

bool validarFecha(const char* fecha) {
    if (strlen(fecha) != 10) return false;
    if (fecha[4] != '-' || fecha[7] != '-') return false;
    
    int anio = atoi(fecha);
    int mes = atoi(fecha + 5);
    int dia = atoi(fecha + 8);
    
    if (anio < 1900 || anio > 2100) return false;
    if (mes < 1 || mes > 12) return false;
    if (dia < 1 || dia > 31) return false;
    
    return true;
}

bool validarHora(const char* hora) {
    if (strlen(hora) != 5) return false;
    if (hora[2] != ':') return false;
    
    int horas = atoi(hora);
    int minutos = atoi(hora + 3);
    
    if (horas < 0 || horas > 23) return false;
    if (minutos < 0 || minutos > 59) return false;
    
    return true;
}

// ============================================================================
// FUNCIONES DE INICIALIZACIÓN Y MEMORIA
// ============================================================================

Hospital* inicializarHospital(const char* nombre, const char* direccion, const char* telefono) {
    Hospital* hospital = new Hospital;
    
    strcpy(hospital->nombre, nombre);
    strcpy(hospital->direccion, direccion);
    strcpy(hospital->telefono, telefono);
    
    hospital->capacidadPacientes = 10;
    hospital->cantidadPacientes = 0;
    hospital->pacientes = new Paciente[hospital->capacidadPacientes];
    
    hospital->capacidadDoctores = 10;
    hospital->cantidadDoctores = 0;
    hospital->doctores = new Doctor[hospital->capacidadDoctores];
    
    hospital->capacidadCitas = 20;
    hospital->cantidadCitas = 0;
    hospital->citas = new Cita[hospital->capacidadCitas];
    
    hospital->siguienteIdPaciente = 1;
    hospital->siguienteIdDoctor = 1;
    hospital->siguienteIdCita = 1;
    hospital->siguienteIdConsulta = 1;
    
    cout << "? Hospital '" << nombre << "' inicializado correctamente." << endl;
    return hospital;
}

void destruirHospital(Hospital* hospital) {
    cout << "\n?? Liberando memoria del sistema hospitalario..." << endl;
    
    for (int i = 0; i < hospital->cantidadPacientes; i++) {
        delete[] hospital->pacientes[i].historial;
        delete[] hospital->pacientes[i].citasAgendadas;
    }
    delete[] hospital->pacientes;
    cout << "? Memoria de pacientes liberada." << endl;
    
    for (int i = 0; i < hospital->cantidadDoctores; i++) {
        delete[] hospital->doctores[i].pacientesAsignados;
        delete[] hospital->doctores[i].citasAgendadas;
    }
    delete[] hospital->doctores;
    cout << "? Memoria de doctores liberada." << endl;
    
    delete[] hospital->citas;
    cout << "? Memoria de citas liberada." << endl;
    
    delete hospital;
    cout << "? Sistema hospitalario cerrado correctamente." << endl;
}

// ============================================================================
// GESTIÓN DE PACIENTES
// ============================================================================

void redimensionarArrayPacientes(Hospital* hospital) {
    int nuevaCapacidad = hospital->capacidadPacientes * 2;
    Paciente* nuevoArray = new Paciente[nuevaCapacidad];
    
    for (int i = 0; i < hospital->cantidadPacientes; i++) {
        nuevoArray[i] = hospital->pacientes[i];
    }
    
    delete[] hospital->pacientes;
    hospital->pacientes = nuevoArray;
    hospital->capacidadPacientes = nuevaCapacidad;
    
    cout << "?? Array de pacientes redimensionado a capacidad: " << nuevaCapacidad << endl;
}

Paciente* buscarPacientePorCedula(Hospital* hospital, const char* cedula) {
    for (int i = 0; i < hospital->cantidadPacientes; i++) {
        if (strcmp(hospital->pacientes[i].cedula, cedula) == 0 && hospital->pacientes[i].activo) {
            return &hospital->pacientes[i];
        }
    }
    return nullptr;
}

Paciente* buscarPacientePorId(Hospital* hospital, int id) {
    for (int i = 0; i < hospital->cantidadPacientes; i++) {
        if (hospital->pacientes[i].id == id && hospital->pacientes[i].activo) {
            return &hospital->pacientes[i];
        }
    }
    return nullptr;
}

Paciente* crearPaciente(Hospital* hospital) {
    if (hospital->cantidadPacientes >= hospital->capacidadPacientes) {
        redimensionarArrayPacientes(hospital);
    }
    
    Paciente* nuevoPaciente = &hospital->pacientes[hospital->cantidadPacientes];
    
    cout << "\n=== REGISTRO DE NUEVO PACIENTE ===" << endl;
    
    nuevoPaciente->id = hospital->siguienteIdPaciente++;
    
    cout << "Nombre: ";
    cin.getline(nuevoPaciente->nombre, 50);
    
    cout << "Apellido: ";
    cin.getline(nuevoPaciente->apellido, 50);
    
    char cedula[20];
    do {
        cout << "Cédula: ";
        cin.getline(cedula, 20);
        if (buscarPacientePorCedula(hospital, cedula) != nullptr) {
            cout << "? Error: Ya existe un paciente con esta cédula." << endl;
        } else {
            strcpy(nuevoPaciente->cedula, cedula);
            break;
        }
    } while (true);
    
    cout << "Edad: ";
    cin >> nuevoPaciente->edad;
    limpiarBuffer();
    
    cout << "Sexo (M/F): ";
    cin >> nuevoPaciente->sexo;
    limpiarBuffer();
    
    cout << "Tipo de sangre: ";
    cin.getline(nuevoPaciente->tipoSangre, 5);
    
    cout << "Teléfono: ";
    cin.getline(nuevoPaciente->telefono, 15);
    
    cout << "Dirección: ";
    cin.getline(nuevoPaciente->direccion, 100);
    
    cout << "Email: ";
    cin.getline(nuevoPaciente->email, 50);
    
    nuevoPaciente->capacidadHistorial = 5;
    nuevoPaciente->cantidadConsultas = 0;
    nuevoPaciente->historial = new HistorialMedico[nuevoPaciente->capacidadHistorial];
    
    nuevoPaciente->capacidadCitas = 5;
    nuevoPaciente->cantidadCitas = 0;
    nuevoPaciente->citasAgendadas = new int[nuevoPaciente->capacidadCitas];
    
    strcpy(nuevoPaciente->alergias, "");
    strcpy(nuevoPaciente->observaciones, "");
    nuevoPaciente->activo = true;
    
    hospital->cantidadPacientes++;
    
    cout << "? Paciente registrado exitosamente. ID: " << nuevoPaciente->id << endl;
    return nuevoPaciente;
}

void listarPacientes(Hospital* hospital) {
    if (hospital->cantidadPacientes == 0) {
        cout << "?? No hay pacientes registrados en el sistema." << endl;
        return;
    }
    
    cout << "\n+------------------------------------------------------------+" << endl;
    cout << "¦                    LISTA DE PACIENTES                      ¦" << endl;
    cout << "¦------------------------------------------------------------¦" << endl;
    cout << "¦ ID  ¦ NOMBRE COMPLETO     ¦ CÉDULA       ¦ EDAD ¦ CONSULTAS¦" << endl;
    cout << "¦-----+---------------------+--------------+------+----------¦" << endl;
    
    int pacientesActivos = 0;
    for (int i = 0; i < hospital->cantidadPacientes; i++) {
        if (hospital->pacientes[i].activo) {
            cout << "¦ " << setw(3) << hospital->pacientes[i].id << " ¦ "
                 << setw(19) << left << (string(hospital->pacientes[i].nombre) + " " + hospital->pacientes[i].apellido) << " ¦ "
                 << setw(12) << hospital->pacientes[i].cedula << " ¦ "
                 << setw(4) << hospital->pacientes[i].edad << " ¦ "
                 << setw(8) << hospital->pacientes[i].cantidadConsultas << "¦" << endl;
            pacientesActivos++;
        }
    }
    cout << "+------------------------------------------------------------+" << endl;
    cout << "Total de pacientes activos: " << pacientesActivos << endl;
}

// ============================================================================
// GESTIÓN DE DOCTORES
// ============================================================================

void redimensionarArrayDoctores(Hospital* hospital) {
    int nuevaCapacidad = hospital->capacidadDoctores * 2;
    Doctor* nuevoArray = new Doctor[nuevaCapacidad];
    
    for (int i = 0; i < hospital->cantidadDoctores; i++) {
        nuevoArray[i] = hospital->doctores[i];
    }
    
    delete[] hospital->doctores;
    hospital->doctores = nuevoArray;
    hospital->capacidadDoctores = nuevaCapacidad;
    
    cout << "?? Array de doctores redimensionado a capacidad: " << nuevaCapacidad << endl;
}

Doctor* buscarDoctorPorId(Hospital* hospital, int id) {
    for (int i = 0; i < hospital->cantidadDoctores; i++) {
        if (hospital->doctores[i].id == id && hospital->doctores[i].disponible) {
            return &hospital->doctores[i];
        }
    }
    return nullptr;
}

Doctor* crearDoctor(Hospital* hospital) {
    if (hospital->cantidadDoctores >= hospital->capacidadDoctores) {
        redimensionarArrayDoctores(hospital);
    }
    
    Doctor* nuevoDoctor = &hospital->doctores[hospital->cantidadDoctores];
    
    cout << "\n=== REGISTRO DE NUEVO DOCTOR ===" << endl;
    
    nuevoDoctor->id = hospital->siguienteIdDoctor++;
    
    cout << "Nombre: ";
    cin.getline(nuevoDoctor->nombre, 50);
    
    cout << "Apellido: ";
    cin.getline(nuevoDoctor->apellido, 50);
    
    cout << "Cédula profesional: ";
    cin.getline(nuevoDoctor->cedula, 20);
    
    cout << "Especialidad: ";
    cin.getline(nuevoDoctor->especialidad, 50);
    
    cout << "Años de experiencia: ";
    cin >> nuevoDoctor->aniosExperiencia;
    limpiarBuffer();
    
    cout << "Costo de consulta: ";
    cin >> nuevoDoctor->costoConsulta;
    limpiarBuffer();
    
    cout << "Horario de atención: ";
    cin.getline(nuevoDoctor->horarioAtencion, 50);
    
    cout << "Teléfono: ";
    cin.getline(nuevoDoctor->telefono, 15);
    
    cout << "Email: ";
    cin.getline(nuevoDoctor->email, 50);
    
    nuevoDoctor->capacidadPacientes = 5;
    nuevoDoctor->cantidadPacientes = 0;
    nuevoDoctor->pacientesAsignados = new int[nuevoDoctor->capacidadPacientes];
    
    nuevoDoctor->capacidadCitas = 10;
    nuevoDoctor->cantidadCitas = 0;
    nuevoDoctor->citasAgendadas = new int[nuevoDoctor->capacidadCitas];
    
    nuevoDoctor->disponible = true;
    
    hospital->cantidadDoctores++;
    
    cout << "? Doctor registrado exitosamente. ID: " << nuevoDoctor->id << endl;
    return nuevoDoctor;
}

void listarDoctores(Hospital* hospital) {
    if (hospital->cantidadDoctores == 0) {
        cout << "?? No hay doctores registrados en el sistema." << endl;
        return;
    }
    
    cout << "\n+----------------------------------------------------------------------+" << endl;
    cout << "¦                         LISTA DE DOCTORES                           ¦" << endl;
    cout << "¦---------------------------------------------------------------------¦" << endl;
    cout << "¦ ID  ¦ NOMBRE COMPLETO     ¦ ESPECIALIDAD     ¦ EXP  ¦ COSTO CONSULTA¦" << endl;
    cout << "¦-----+---------------------+------------------+------+---------------¦" << endl;
    
    int doctoresActivos = 0;
    for (int i = 0; i < hospital->cantidadDoctores; i++) {
        if (hospital->doctores[i].disponible) {
            cout << "¦ " << setw(3) << hospital->doctores[i].id << " ¦ "
                 << setw(19) << left << (string(hospital->doctores[i].nombre) + " " + hospital->doctores[i].apellido) << " ¦ "
                 << setw(16) << hospital->doctores[i].especialidad << " ¦ "
                 << setw(4) << hospital->doctores[i].aniosExperiencia << " ¦ "
                 << setw(13) << fixed << setprecision(2) << hospital->doctores[i].costoConsulta << " ¦" << endl;
            doctoresActivos++;
        }
    }
    cout << "+---------------------------------------------------------------------+" << endl;
    cout << "Total de doctores activos: " << doctoresActivos << endl;
}

// ============================================================================
// GESTIÓN DE CITAS
// ============================================================================

void redimensionarArrayCitas(Hospital* hospital) {
    int nuevaCapacidad = hospital->capacidadCitas * 2;
    Cita* nuevoArray = new Cita[nuevaCapacidad];
    
    for (int i = 0; i < hospital->cantidadCitas; i++) {
        nuevoArray[i] = hospital->citas[i];
    }
    
    delete[] hospital->citas;
    hospital->citas = nuevoArray;
    hospital->capacidadCitas = nuevaCapacidad;
    
    cout << "?? Array de citas redimensionado a capacidad: " << nuevaCapacidad << endl;
}

bool verificarDisponibilidad(Hospital* hospital, int idDoctor, const char* fecha, const char* hora) {
    for (int i = 0; i < hospital->cantidadCitas; i++) {
        if (hospital->citas[i].idDoctor == idDoctor &&
            strcmp(hospital->citas[i].fecha, fecha) == 0 &&
            strcmp(hospital->citas[i].hora, hora) == 0 &&
            strcmp(hospital->citas[i].estado, "Cancelada") != 0) {
            return false;
        }
    }
    return true;
}

Cita* agendarCita(Hospital* hospital) {
    if (hospital->cantidadCitas >= hospital->capacidadCitas) {
        redimensionarArrayCitas(hospital);
    }
    
    Cita* nuevaCita = &hospital->citas[hospital->cantidadCitas];
    
    cout << "\n=== AGENDAR NUEVA CITA ===" << endl;
    
    listarPacientes(hospital);
    cout << "ID del paciente: ";
    cin >> nuevaCita->idPaciente;
    limpiarBuffer();
    
    Paciente* paciente = buscarPacientePorId(hospital, nuevaCita->idPaciente);
    if (paciente == nullptr) {
        cout << "Error: Paciente no encontrado." << endl;
        return nullptr;
    }
    
    listarDoctores(hospital);
    cout << "ID del doctor: ";
    cin >> nuevaCita->idDoctor;
    limpiarBuffer();
    
    Doctor* doctor = buscarDoctorPorId(hospital, nuevaCita->idDoctor);
    if (doctor == nullptr) {
        cout << "Error: Doctor no encontrado." << endl;
        return nullptr;
    }
    
    char fecha[11], hora[6];
    do {
        cout << "Fecha (YYYY-MM-DD): ";
        cin.getline(fecha, 11);
        if (!validarFecha(fecha)) {
            cout << "Formato de fecha inválido." << endl;
        } else {
            strcpy(nuevaCita->fecha, fecha);
            break;
        }
    } while (true);
    
    do {
        cout << "Hora (HH:MM): ";
        cin.getline(hora, 6);
        if (!validarHora(hora)) {
            cout << "Formato de hora inválido." << endl;
        } else if (!verificarDisponibilidad(hospital, nuevaCita->idDoctor, fecha, hora)) {
            cout << "Doctor no disponible en ese horario." << endl;
        } else {
            strcpy(nuevaCita->hora, hora);
            break;
        }
    } while (true);
    
    cout << "Motivo de la consulta: ";
    cin.getline(nuevaCita->motivo, 150);
    
    nuevaCita->id = hospital->siguienteIdCita++;
    strcpy(nuevaCita->estado, "Agendada");
    strcpy(nuevaCita->observaciones, "");
    nuevaCita->atendida = false;
    
    if (paciente->cantidadCitas >= paciente->capacidadCitas) {
        int nuevaCap = paciente->capacidadCitas * 2;
        int* nuevoArray = new int[nuevaCap];
        for (int i = 0; i < paciente->cantidadCitas; i++) {
            nuevoArray[i] = paciente->citasAgendadas[i];
        }
        delete[] paciente->citasAgendadas;
        paciente->citasAgendadas = nuevoArray;
        paciente->capacidadCitas = nuevaCap;
    }
    paciente->citasAgendadas[paciente->cantidadCitas++] = nuevaCita->id;
    
    if (doctor->cantidadCitas >= doctor->capacidadCitas) {
        int nuevaCap = doctor->capacidadCitas * 2;
        int* nuevoArray = new int[nuevaCap];
        for (int i = 0; i < doctor->cantidadCitas; i++) {
            nuevoArray[i] = doctor->citasAgendadas[i];
        }
        delete[] doctor->citasAgendadas;
        doctor->citasAgendadas = nuevoArray;
        doctor->capacidadCitas = nuevaCap;
    }
    doctor->citasAgendadas[doctor->cantidadCitas++] = nuevaCita->id;
    
    hospital->cantidadCitas++;
    
    cout << "Cita agendada exitosamente. ID: " << nuevaCita->id << endl;
    cout << "Paciente: " << paciente->nombre << " " << paciente->apellido << endl;
    cout << "Doctor: " << doctor->nombre << " " << doctor->apellido << endl;
    cout << "Fecha: " << nuevaCita->fecha << " " << nuevaCita->hora << endl;
    
    return nuevaCita;
}

// ============================================================================
// GESTIÓN DE HISTORIAL MÉDICO
// ============================================================================

void agregarConsultaAlHistorial(Paciente* paciente, HistorialMedico consulta) {
    if (paciente->cantidadConsultas >= paciente->capacidadHistorial) {
        int nuevaCapacidad = paciente->capacidadHistorial * 2;
        HistorialMedico* nuevoHistorial = new HistorialMedico[nuevaCapacidad];
        
        for (int i = 0; i < paciente->cantidadConsultas; i++) {
            nuevoHistorial[i] = paciente->historial[i];
        }
        
        delete[] paciente->historial;
        paciente->historial = nuevoHistorial;
        paciente->capacidadHistorial = nuevaCapacidad;
        
        cout << "Historial médico redimensionado a: " << nuevaCapacidad << " consultas" << endl;
    }
    
    paciente->historial[paciente->cantidadConsultas] = consulta;
    paciente->cantidadConsultas++;
    
    cout << "Consulta agregada al historial médico." << endl;
}

void mostrarHistorialMedico(Paciente* paciente) {
    if (paciente->cantidadConsultas == 0) {
        cout << "El paciente no tiene consultas en su historial." << endl;
        return;
    }
    
    cout << "\n+------------------------------------------------------------------------------+" << endl;
    cout << "¦                       HISTORIAL MÉDICO - " << paciente->nombre << " " << paciente->apellido;
    // Ajustar espacios para alineación
    int espacios = 50 - strlen(paciente->nombre) - strlen(paciente->apellido);
    for (int i = 0; i < espacios; i++) cout << " ";
    cout << "¦" << endl;
    cout << "¦----------------------------------------------------------------------------¦" << endl;
    cout << "¦ CONSUL ¦ FECHA      ¦ HORA   ¦ DIAGNÓSTICO              ¦ COSTO           ¦" << endl;
    cout << "¦--------+------------+--------+--------------------------+------------------¦" << endl;
    
    for (int i = 0; i < paciente->cantidadConsultas; i++) {
        cout << "¦ " << setw(6) << paciente->historial[i].idConsulta << " ¦ "
             << setw(10) << paciente->historial[i].fecha << " ¦ "
             << setw(6) << paciente->historial[i].hora << " ¦ "
             << setw(24) << left << paciente->historial[i].diagnostico << " ¦ "
             << setw(14) << fixed << setprecision(2) << paciente->historial[i].costo << " ¦" << endl;
    }
    cout << "+----------------------------------------------------------------------------+" << endl;
    cout << "Total de consultas: " << paciente->cantidadConsultas << endl;
}

#endif //FUNCIONES_H
