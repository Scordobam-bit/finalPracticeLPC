#ifndef SENIORPASSENGER_H
#define SENIORPASSENGER_H

#include "Passenger.h" // Incluye la clase base Passenger

using namespace std;

class SeniorPassenger : public Passenger { // Clase SeniorPassenger que hereda de Passenger
public:
    SeniorPassenger() = default; // Constructor por defecto
    virtual ~SeniorPassenger() = default; // Destructor virtual por defecto

protected:
    long long computeFare(const TransportMode& mode) const override; // Sobreescribe el calculo de tarifa
};

#endif