#include "StudentPassenger.h" // Incluye la cabecera de StudentPassenger

using namespace std;

long long StudentPassenger::computeFare(const TransportMode& mode) const { // Sobreescribe computeFare para estudiante
    if (mode.allowsStudentFlatFare()) { // Consulta polimorficamente si el modo permite tarifa plana de estudiante
        return 1600LL; // Retorna la tarifa plana de 1600 para estudiante si el modo lo permite
    }
    return Passenger::computeFare(mode); // Llama al metodo de la clase base Passenger si el modo no permite tarifa plana
} 