#ifndef FREQUENTPASSENGER_H
#define FREQUENTPASSENGER_H

#include "Passenger.h" // Incluye la clase base Passenger

using namespace std;

class FrequentPassenger : public Passenger { // Clase FrequentPassenger que hereda de Passenger
public:
    FrequentPassenger() = default; // Constructor por defecto
    virtual ~FrequentPassenger() = default; // Destructor virtual por defecto
    // No sobreescribe nada: hereda el comportamiento por defecto de Passenger (tarifa plena)
};

#endif