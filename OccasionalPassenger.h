#ifndef OCCASIONALPASSENGER_H
#define OCCASIONALPASSENGER_H

#include "Passenger.h" // Incluye la clase base Passenger

using namespace std;

class OccasionalPassenger : public Passenger { // Clase OccasionalPassenger que hereda de Passenger
public:
    OccasionalPassenger() = default; // Constructor por defecto
    virtual ~OccasionalPassenger() = default; // Destructor virtual por defecto

protected:
    long long computeFare(const TransportMode& mode) const override; // Sobreescribe el calculo de tarifa
};

#endif