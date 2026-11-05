#include "OccasionalPassenger.h" // Incluye la cabecera de OccasionalPassenger

using namespace std;

long long OccasionalPassenger::computeFare(const TransportMode& mode) const { // Sobreescribe computeFare para pasajero ocasional
    return Passenger::computeFare(mode) + 580LL; // Suma 580 a la tarifa base obtenida mediante Passenger::computeFare
}