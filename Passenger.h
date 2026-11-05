#ifndef PASSENGER_H
#define PASSENGER_H

#include "TransportMode.h" // Incluye la definicion de TransportMode
#include <algorithm> // Incluye la libreria de algoritmos estandar para std::max

using namespace std;

class Passenger { // Clase base Passenger implementando el patron Non-Virtual Interface (NVI)
public:
    Passenger() = default; // Constructor por defecto
    virtual ~Passenger() = default; // Destructor virtual por defecto para polimorfismo

    long long fareOn(const TransportMode& mode) const; // Metodo no virtual publico de la interfaz NVI

protected:
    virtual long long computeFare(const TransportMode& mode) const; // Metodo virtual protegido con comportamiento por defecto
};

#endif