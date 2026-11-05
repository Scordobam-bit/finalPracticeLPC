#include "FeederRoute.h" // Incluye la cabecera de FeederRoute

using namespace std;

FeederRoute::FeederRoute(const string& nombre, int estaciones) // Constructor de FeederRoute
    : TransportMode(nombre, estaciones) {} // Llama al constructor de la clase base TransportMode

long long FeederRoute::baseFare() const { // Sobreescribe baseFare para FeederRoute
    return 0LL; // Retorna tarifa cero ya que la ruta alimentadora es gratuita por defecto
}

string FeederRoute::serviceType() const { // Sobreescribe serviceType para FeederRoute
    return "Feeder route"; // Retorna el texto exacto que representa la ruta alimentadora
}

bool FeederRoute::opensIntegrationWindow() const { // Sobreescribe hook para apertura de ventana de integracion
    return false; // Retorna false para evitar que una ruta alimentadora inicie una ventana de integracion
}