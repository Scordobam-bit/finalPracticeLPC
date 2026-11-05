#include "TouristCable.h" // Incluye la cabecera de TouristCable

using namespace std;

TouristCable::TouristCable(const string& nombre, int estaciones, long long valorRecargo) // Constructor de TouristCable
    : CableLine(nombre, estaciones), recargo(valorRecargo) {} // Inicializa la clase padre CableLine y el recargo

long long TouristCable::baseFare() const { // Sobreescribe baseFare para TouristCable
    return CableLine::baseFare() + recargo; // Suma el recargo a la tarifa de CableLine sin usar el literal 3820
}

string TouristCable::serviceType() const { // Sobreescribe serviceType para TouristCable
    return "Tourist cable"; // Retorna el texto exacto para tipo de servicio de cable turistico
}

bool TouristCable::allowsStudentFlatFare() const { // Sobreescribe hook para tarifa de estudiante
    return false; // Retorna false indicando que no admite tarifa plana de estudiante
}

bool TouristCable::allowsIntegration() const { // Sobreescribe hook para integracion tarifaria
    return false; // Retorna false indicando que no admite integracion
}

bool TouristCable::opensIntegrationWindow() const { // Sobreescribe hook para ventana de integracion
    return false; // Retorna false indicando que no abre ventana de integracion
}