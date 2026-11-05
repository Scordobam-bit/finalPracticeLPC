#include "MetroLine.h" // Incluye la cabecera de MetroLine

using namespace std;

MetroLine::MetroLine(const string& nombre, int estaciones) // Constructor de MetroLine
    : TransportMode(nombre, estaciones) {} // Llama al constructor de la clase base TransportMode

long long MetroLine::baseFare() const { // Sobreescribe baseFare para MetroLine
    return 3820LL; // Retorna la tarifa base plena de 3820 (Ocurrencia 1 de 3 del literal 3820)
}

string MetroLine::serviceType() const { // Sobreescribe serviceType para MetroLine
    return "Metro line"; // Retorna el texto exacto que representa el tipo de servicio de metro
}