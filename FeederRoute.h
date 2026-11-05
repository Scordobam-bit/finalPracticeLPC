#ifndef FEEDERROUTE_H
#define FEEDERROUTE_H

#include "TransportMode.h" // Incluye la clase base TransportMode

using namespace std;

class FeederRoute : public TransportMode { // Clase FeederRoute que hereda de TransportMode
public:
    FeederRoute(const string& nombre, int estaciones); // Constructor con parametros para FeederRoute
    virtual ~FeederRoute() = default; // Destructor virtual por defecto

    long long baseFare() const override; // Sobreescribe metodo para retornar tarifa cero
    string serviceType() const override; // Sobreescribe metodo para retornar tipo de servicio alimentador

    bool opensIntegrationWindow() const override; // Sobreescribe hook para no abrir ventana de integracion
};

#endif