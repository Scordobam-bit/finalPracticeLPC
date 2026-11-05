#ifndef METROLINE_H
#define METROLINE_H

#include "TransportMode.h" // Incluye la clase base TransportMode

using namespace std;

class MetroLine : public TransportMode { // Clase MetroLine que hereda de TransportMode
public:
    MetroLine(const string& nombre, int estaciones); // Constructor con parametros para MetroLine
    virtual ~MetroLine() = default; // Destructor virtual por defecto

    long long baseFare() const override; // Sobreescribe metodo para retornar la tarifa base de metro
    string serviceType() const override; // Sobreescribe metodo para retornar el tipo de servicio de metro
};

#endif