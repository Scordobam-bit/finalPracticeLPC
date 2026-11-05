#ifndef TRAMLINE_H
#define TRAMLINE_H

#include "TransportMode.h" // Incluye la clase base TransportMode

using namespace std;

class TramLine : public TransportMode { // Clase TramLine que hereda de TransportMode
public:
    TramLine(const string& nombre, int estaciones); // Constructor con parametros para TramLine
    virtual ~TramLine() = default; // Destructor virtual por defecto

    long long baseFare() const override; // Sobreescribe metodo para retornar la tarifa base de tranvia
    string serviceType() const override; // Sobreescribe metodo para retornar el tipo de servicio de tranvia
};

#endif