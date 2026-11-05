#ifndef TOURISTCABLE_H
#define TOURISTCABLE_H

#include "CableLine.h" // Incluye la clase padre CableLine

using namespace std;

class TouristCable : public CableLine { // Clase TouristCable que hereda directamente de CableLine
private:
    long long recargo; // Atributo privado para almacenar el recargo especifico del cable turistico

public:
    TouristCable(const string& nombre, int estaciones, long long valorRecargo); // Constructor con recargo
    virtual ~TouristCable() = default; // Destructor virtual por defecto

    long long baseFare() const override; // Sobreescribe baseFare reutilizando la tarifa de CableLine
    string serviceType() const override; // Sobreescribe serviceType para indicar que es cable turistico

    bool allowsStudentFlatFare() const override; // Sobreescribe hook retornando false
    bool allowsIntegration() const override; // Sobreescribe hook retornando false
    bool opensIntegrationWindow() const override; // Sobreescribe hook retornando false
};

#endif