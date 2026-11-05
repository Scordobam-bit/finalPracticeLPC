#include "SeniorPassenger.h" // Incluye la cabecera de SeniorPassenger

using namespace std;

long long SeniorPassenger::computeFare(const TransportMode& mode) const { // Sobreescribe computeFare para adulto mayor
    return Passenger::computeFare(mode) - 490LL; // Descuenta 490 de la tarifa base obtenida mediante Passenger::computeFare
}