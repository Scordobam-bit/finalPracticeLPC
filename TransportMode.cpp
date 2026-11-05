#include "TransportMode.h" // Incluye la cabecera correspondiente a TransportMode

using namespace std;

TransportMode::TransportMode(const string& nombre, int estaciones) // Constructor de la clase base TransportMode
    : nombreModo(nombre), numeroEstaciones(estaciones) {} // Inicializa los atributos nombreModo y numeroEstaciones

bool TransportMode::allowsStudentFlatFare() const { // Implementacion por defecto del hook para tarifa plana de estudiante
    return true; // Retorna true por defecto indicando que el modo permite tarifa plana de estudiante
}

bool TransportMode::allowsIntegration() const { // Implementacion por defecto del hook para integracion tarifaria
    return true; // Retorna true por defecto indicando que el modo admite integracion
}

bool TransportMode::opensIntegrationWindow() const { // Implementacion por defecto del hook para apertura de ventana
    return true; // Retorna true por defecto indicando que el modo abre ventana de integracion
}

string TransportMode::describe() const { // Implementacion del metodo no virtual describe (Template Method)
    return serviceType() + " " + nombreModo + " (" + to_string(numeroEstaciones) + " stations) full fare " + formatCOP(baseFare()); // Construye y retorna la cadena descriptiva formateada
}