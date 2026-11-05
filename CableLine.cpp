#include "CableLine.h" // Incluye la cabecera de CableLine

using namespace std;

CableLine::CableLine(const string& nombre, int estaciones) // Constructor de CableLine
    : TransportMode(nombre, estaciones) {} // Llama al constructor de la clase base TransportMode

long long CableLine::baseFare() const { // Sobreescribe baseFare para CableLine
    return 3820LL; // Retorna la tarifa base plena de 3820 (Ocurrencia 3 de 3 del literal 3820)
}

string CableLine::serviceType() const { // Sobreescribe serviceType para CableLine
    return "Cable line"; // Retorna el texto exacto que representa el tipo de servicio de cable
}