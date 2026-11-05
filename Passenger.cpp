#include "Passenger.h" // Incluye la cabecera de Passenger

using namespace std;

long long Passenger::fareOn(const TransportMode& mode) const { // Metodo NVI publico no virtual
    return max(0LL, computeFare(mode)); // Garantiza que la tarifa calculada nunca sea negativa usando std::max
} 

long long Passenger::computeFare(const TransportMode& mode) const { // Metodo virtual protegido base
    return mode.baseFare(); // Comportamiento por defecto: retorna la tarifa plena del modo de transporte
}