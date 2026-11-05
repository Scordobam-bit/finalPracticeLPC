#ifndef CABLELINE_H
#define CABLELINE_H

#include "TransportMode.h" // Incluye la clase base TransportMode

using namespace std;

class CableLine : public TransportMode { // Clase CableLine que hereda de TransportMode
public:
    CableLine(const string& nombre, int estaciones); // Constructor con parametros para CableLine
    virtual ~CableLine() = default; // Destructor virtual por defecto

    long long baseFare() const override; // Sobreescribe metodo para retornar la tarifa base de cable
    string serviceType() const override; // Sobreescribe metodo para retornar el tipo de servicio de cable
};

#endif