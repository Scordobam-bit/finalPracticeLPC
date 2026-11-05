#ifndef STUDENTPASSENGER_H
#define STUDENTPASSENGER_H

#include "Passenger.h" // Incluye la clase base Passenger

using namespace std;

class StudentPassenger : public Passenger { // Clase StudentPassenger que hereda de Passenger
public:
    StudentPassenger() = default; // Constructor por defecto
    virtual ~StudentPassenger() = default; // Destructor virtual por defecto

protected:
    long long computeFare(const TransportMode& mode) const override; // Sobreescribe el calculo de tarifa
};

#endif