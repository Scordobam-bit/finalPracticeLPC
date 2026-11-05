#include "TramLine.h" // Incluye la cabecera de TramLine

using namespace std;

TramLine::TramLine(const string& nombre, int estaciones) // Constructor de TramLine
    : TransportMode(nombre, estaciones) {} // Llama al constructor de la clase base TransportMode

long long TramLine::baseFare() const { // Sobreescribe baseFare para TramLine
    return 3820LL; // Retorna la tarifa base plena de 3820 (Ocurrencia 2 de 3 del literal 3820)
}

string TramLine::serviceType() const { // Sobreescribe serviceType para TramLine
    return "Tram line"; // Retorna el texto exacto que representa el tipo de servicio de tranvia
}