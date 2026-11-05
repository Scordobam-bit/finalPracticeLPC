#include "DisabledPassenger.h" // Incluye la cabecera de DisabledPassenger

using namespace std;

long long DisabledPassenger::computeFare(const TransportMode& mode) const { // Sobreescribe computeFare para persona con discapacidad
    return Passenger::computeFare(mode) - 1100LL; // Descuenta 1100 de la tarifa base obtenida mediante Passenger::computeFare
}