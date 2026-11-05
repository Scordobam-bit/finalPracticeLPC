#ifndef DISABLEDPASSENGER_H
#define DISABLEDPASSENGER_H

#include "Passenger.h" // Incluye la clase base Passenger

using namespace std;

class DisabledPassenger : public Passenger { // Clase DisabledPassenger que hereda de Passenger
public:
    DisabledPassenger() = default; // Constructor por defecto
    virtual ~DisabledPassenger() = default; // Destructor virtual por defecto

protected:
    long long computeFare(const TransportMode& mode) const override; // Sobreescribe el calculo de tarifa
};

#endif