#ifndef TRANSPORTMODE_H
#define TRANSPORTMODE_H

#include <string> 

using namespace std;

string formatCOP(long long value); // Declaracion de la funcion externa para formatear valores monetarios en COP

class TransportMode { // Declaracion de la clase base abstracta TransportMode
protected:
    string nombreModo; // Atributo protegido que almacena el nombre del modo de transporte
    int numeroEstaciones; // Atributo protegido que almacena el numero de estaciones del modo

public:
    TransportMode(const string& nombre, int estaciones); // Constructor con parametros para inicializar atributos
    virtual ~TransportMode() = default; // Destructor virtual por defecto para garantizar destruccion polimorfica limpia

    virtual long long baseFare() const = 0; // Metodo virtual puro para obtener la tarifa base
    virtual string serviceType() const = 0; // Metodo virtual puro para obtener el tipo de servicio

    virtual bool allowsStudentFlatFare() const; // Hook virtual para determinar si aplica tarifa plana de estudiante
    virtual bool allowsIntegration() const; // Hook virtual para determinar si el modo permite integracion
    virtual bool opensIntegrationWindow() const; // Hook virtual para determinar si el modo abre ventana de integracion

    string describe() const; // Metodo no virtual que genera la cadena descriptiva del modo de transporte
};

#endif