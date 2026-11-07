/* =========================================================================
   PRACTICE - STARTING CODE (DO NOT CHANGE THE GIVEN SECTIONS)
   Integrated Itinerary and Fare Engine - Medellin Metro System
   Programming Languages and Paradigms (ST0244) - Universidad EAFIT

   Build: g++ -std=c++17 -Wall -Wextra -o practice main.cpp
   Target environment: OnlineGDB, language selector set to C++17

   WHAT YOU RECEIVE
     - the currency formatter, the Leg and ChargedLeg records and the whole
       main() test harness, so that the expected output is reproducible
     - the declarations you must complete, each marked with TODO

   WHAT YOU MUST WRITE
     - PART 1: the TransportMode hierarchy
     - PART 2: the Passenger hierarchy
     - PART 3: chargeItinerary and the Network class
     - PART 4: the four report functions, with STL algorithms only

   The file must compile with ZERO errors and ZERO warnings under
   g++ -std=c++17 -Wall -Wextra. A warning is a defect, not a detail.
   ========================================================================= */

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <numeric>
#include <map>
#include <functional>

using namespace std;

/* -------------------------------------------------------------------------
   Utility: Colombian peso formatting (3820 -> "$ 3.820")
   ------------------------------------------------------------------------- */
string formatCOP(long long value) {
    string digits = to_string(value); // Convierte el numero a texto
    string out; // String de salida para almacenar el resultado
    int count = 0; // Contador de digitos para agrupacion de miles
    for (auto it = digits.rbegin(); it != digits.rend(); ++it) { // Recorre los digitos de derecha a izquierda
        out.push_back(*it); // Agrega el digito actual al string de salida
        if (++count % 3 == 0 && it + 1 != digits.rend()) { // Evalua si corresponde insertar punto separador
            out.push_back('.'); // Inserta el punto de separacion de miles
        }
    }
    reverse(out.begin(), out.end()); // Invierte la cadena para restaurar el orden correcto
    return "$ " + out; // Retorna la cadena con formato monetario
}

/* -------------------------------------------------------------------------
   PART 1. Abstract base class of the transport-mode hierarchy
   ------------------------------------------------------------------------- */
class TransportMode {
public:
    TransportMode(string name, int stations) // Constructor con parametros de nombre y estaciones
        : name_(move(name)), stations_(stations) {} // Inicializa los atributos de la clase base

    virtual ~TransportMode() = default;          // mandatory: deleted through base pointer

    const string& name() const { return name_; } // Retorna referencia constante al nombre del modo
    int stations() const { return stations_; } // Retorna el numero de estaciones del modo

    virtual long long baseFare() const = 0; // Metodo virtual puro para obtener la tarifa basica
    virtual string serviceType() const = 0; // Metodo virtual puro para obtener el tipo de servicio

    virtual bool allowsStudentFlatFare() const { return true; } // Hook virtual que permite tarifa plana de estudiante por defecto
    virtual bool allowsIntegration() const { return true; } // Hook virtual que autoriza integracion tarifaria por defecto
    virtual bool opensIntegrationWindow() const { return true; } // Hook virtual que autoriza abrir ventana de integracion por defecto

    string describe() const { // Metodo no virtual que genera la descripcion formateada del modo
        return serviceType() + " " + name_ + " (" + to_string(stations_) + " stations) - full fare " + formatCOP(baseFare()); // Construye y retorna la cadena descriptiva
    }

private:
    string name_; // Atributo privado con el nombre del modo
    int stations_; // Atributo privado con la cantidad de estaciones
};

class MetroLine : public TransportMode {
public:
    MetroLine(string name, int stations) // Constructor de la linea de metro
        : TransportMode(move(name), stations) {} // Delega inicializacion a TransportMode

    long long baseFare() const override { return 3820LL; } // Ocurrencia 1 de 3 del literal 3820
    string serviceType() const override { return "Metro line"; } // Retorna el nombre del servicio de metro
};

class TramLine : public TransportMode {
public:
    TramLine(string name, int stations) // Constructor de la linea de tranvia
        : TransportMode(move(name), stations) {} // Delega inicializacion a TransportMode

    long long baseFare() const override { return 3820LL; } // Ocurrencia 2 de 3 del literal 3820
    string serviceType() const override { return "Tram line"; } // Retorna el nombre del servicio de tranvia
};

class CableLine : public TransportMode {
public:
    CableLine(string name, int stations) // Constructor de la linea de metrocable
        : TransportMode(move(name), stations) {} // Delega inicializacion a TransportMode

    long long baseFare() const override { return 3820LL; } // Ocurrencia 3 de 3 del literal 3820
    string serviceType() const override { return "Cable line"; } // Retorna el nombre del servicio de cable
};

class TouristCable : public CableLine {
public:
    TouristCable(string name, int stations, long long surcharge) // Constructor del cable turistico con recargo
        : CableLine(move(name), stations), surcharge_(surcharge) {} // Inicializa la clase CableLine y el recargo

    long long baseFare() const override { return CableLine::baseFare() + surcharge_; } // Reutiliza tarifa base de CableLine sin repetir el literal 3820
    string serviceType() const override { return "Tourist cable"; } // Retorna el nombre del servicio de cable turistico

    bool allowsStudentFlatFare() const override { return false; } // Deniega la tarifa plana de estudiante para este modo
    bool allowsIntegration() const override { return false; } // Deniega la integracion tarifaria para este modo
    bool opensIntegrationWindow() const override { return false; } // Deniega la apertura de ventana de integracion para este modo

private:
    long long surcharge_; // Atributo privado para el valor del recargo
};

class FeederRoute : public TransportMode {
public:
    FeederRoute(string name, int stations) // Constructor de la ruta alimentadora
        : TransportMode(move(name), stations) {} // Delega inicializacion a TransportMode

    long long baseFare() const override { return 0LL; } // Retorna tarifa base cero para rutas alimentadoras
    string serviceType() const override { return "Feeder route"; } // Retorna el nombre del servicio alimentador

    bool allowsStudentFlatFare() const override { return false; } // Deniega la tarifa plana de estudiante para este modo
    bool opensIntegrationWindow() const override { return false; } // Deniega la apertura de ventana de integracion para rutas alimentadoras
};

/* -------------------------------------------------------------------------
   PART 2. Passenger hierarchy, Non-Virtual Interface idiom
   ------------------------------------------------------------------------- */
class Passenger {
public:
    explicit Passenger(string profileName) // Constructor que recibe el nombre del usuario
        : profileName_(move(profileName)) {} // Inicializa el nombre del titular del perfil

    virtual ~Passenger() = default; // Destructor virtual obligatorio para destruccion limpia

    virtual string profile() const = 0; // Metodo virtual puro que retorna la descripcion del perfil

    long long fareOn(const TransportMode& mode) const { // Metodo NVI publico no virtual que evita tarifas negativas
        return max(0LL, computeFare(mode)); // Llama a computeFare y asegura resultado no negativo mediante std::max
    }

protected:
    virtual long long computeFare(const TransportMode& mode) const { // Metodo virtual protegido de calculo base
        return mode.baseFare(); // Comportamiento por defecto que retorna la tarifa basica del modo
    }

    string profileName_; // Atributo protegido con el nombre del usuario
};

class FrequentPassenger : public Passenger {
public:
    explicit FrequentPassenger(string profileName) // Constructor para pasajero frecuente
        : Passenger(move(profileName)) {} // Delega inicializacion a Passenger

    string profile() const override { // Implementa el perfil descriptivo del pasajero frecuente
        return "Civica frequent (" + profileName_ + ")"; // Retorna la cadena de perfil frecuente
    }
};

class StudentPassenger : public Passenger {
public:
    explicit StudentPassenger(string profileName) // Constructor para pasajero estudiante
        : Passenger(move(profileName)) {} // Delega inicializacion a Passenger

    string profile() const override { // Implementa el perfil descriptivo del pasajero estudiante
        return "Municipal student (" + profileName_ + ")"; // Retorna la cadena de perfil estudiante
    }

protected:
    long long computeFare(const TransportMode& mode) const override { // Sobreescribe el calculo de tarifa para estudiantes
        if (mode.allowsStudentFlatFare()) { // Evalua polimorficamente si el modo admite tarifa plana
            return 1600LL; // Retorna tarifa plana de 1600 pesos si el modo la permite
        }
        return Passenger::computeFare(mode); // Retorna la tarifa basica completa si el modo no permite tarifa plana
    }
};

class SeniorPassenger : public Passenger {
public:
    explicit SeniorPassenger(string profileName) // Constructor para pasajero adulto mayor
        : Passenger(move(profileName)) {} // Delega inicializacion a Passenger

    string profile() const override { // Implementa el perfil descriptivo del pasajero adulto mayor
        return "Senior citizen (" + profileName_ + ")"; // Retorna la cadena de perfil adulto mayor
    }

protected:
    long long computeFare(const TransportMode& mode) const override { // Sobreescribe el calculo de tarifa para adulto mayor
        if (!mode.allowsIntegration()) { // Verifica si el modo de transporte permite integracion o beneficios sociales
            return Passenger::computeFare(mode); // Retorna la tarifa base completa sin descuento en modos no integrados
        }
        return Passenger::computeFare(mode) - 490LL; // Descuenta 490 pesos a la tarifa basica obtenida de la clase base
    }
};

class DisabilityPassenger : public Passenger {
public:
    explicit DisabilityPassenger(string profileName) // Constructor para pasajero con discapacidad
        : Passenger(move(profileName)) {} // Delega inicializacion a Passenger

    string profile() const override { // Implementa el perfil descriptivo de persona con discapacidad
        return "Person with disability (" + profileName_ + ")"; // Retorna la cadena de perfil discapacidad
    }

protected:
    long long computeFare(const TransportMode& mode) const override { // Sobreescribe el calculo de tarifa para discapacidad
        if (!mode.allowsIntegration()) { // Verifica si el modo de transporte permite integracion o beneficios sociales
            return Passenger::computeFare(mode); // Retorna la tarifa base completa sin descuento en modos no integrados
        }
        return Passenger::computeFare(mode) - 1100LL; // Descuenta 1100 pesos a la tarifa basica obtenida de la clase base
    }
};

class BearerPassenger : public Passenger {
public:
    explicit BearerPassenger(string profileName) // Constructor para pasajero ocasional o portador
        : Passenger(move(profileName)) {} // Delega inicializacion a Passenger

    string profile() const override { // Implementa el perfil descriptivo del pasajero ocasional
        return "Bearer (occasional) (" + profileName_ + ")"; // Retorna la cadena de perfil ocasional
    }

protected:
    long long computeFare(const TransportMode& mode) const override { // Sobreescribe el calculo de tarifa para pasajero ocasional
        if (!mode.allowsIntegration()) { // Verifica si el modo de transporte permite integracion o recargos tarifarios especiales
            return Passenger::computeFare(mode); // Retorna la tarifa base completa en modos no integrados
        }
        return Passenger::computeFare(mode) + 580LL; // Adiciona 580 pesos de recargo a la tarifa basica obtenida de la clase base
    }
};

/* -------------------------------------------------------------------------
   MAIN PRUEBA INTEGRANTE 1
   ------------------------------------------------------------------------- */
int main() {
    cout << "============================================================\n";
    cout << "    INTEGRATED ITINERARY AND FARE ENGINE - Valle de Aburra\n";
    cout << "============================================================\n\n";

    MetroLine lineA("A", 21); // Instancia Metro Linea A
    MetroLine lineB("B", 7); // Instancia Metro Linea B
    TramLine tramTA("T-A Ayacucho", 9); // Instancia Tranvia Ayacucho
    CableLine cableK("K Santo Domingo", 4); // Instancia Metrocable Linea K
    TouristCable arvi("L Arvi", 2, 9180); // Instancia Cable Turistico Arvi con recargo
    FeederRoute feeder("130 Robledo", 18); // Instancia Ruta Alimentadora 130

    cout << ">> 1. NETWORK INVENTORY (polymorphic describe)\n"; // Imprime titulo de la seccion 1
    cout << "  " << lineA.describe() << '\n'; // Muestra descripcion de Metro Linea A
    cout << "  " << lineB.describe() << '\n'; // Muestra descripcion de Metro Linea B
    cout << "  " << tramTA.describe() << '\n'; // Muestra descripcion de Tranvia
    cout << "  " << cableK.describe() << '\n'; // Muestra descripcion de Cable K
    cout << "  " << arvi.describe() << '\n'; // Muestra descripcion de Cable Arvi
    cout << "  " << feeder.describe() << "\n\n"; // Muestra descripcion de Ruta Alimentadora

    FrequentPassenger frequent("Diego"); // Instancia usuario con perfil frecuente
    StudentPassenger student("Valentina"); // Instancia usuario con perfil estudiante
    SeniorPassenger senior("Dona Amparo"); // Instancia usuario con perfil adulto mayor

    cout << ">> 2. FULL FARE PER PROFILE ON METRO LINE A\n";
    cout << "  " << frequent.profile() << " pays " << formatCOP(frequent.fareOn(lineA)) << '\n'; // Imprime valor a pagar por frecuente
    cout << "  " << student.profile() << " pays " << formatCOP(student.fareOn(lineA)) << '\n'; // Imprime valor a pagar por estudiante
    cout << "  " << senior.profile() << " pays " << formatCOP(senior.fareOn(lineA)) << '\n'; // Imprime valor a pagar por adulto mayor
    cout << "  " << student.profile() << " on the tourist cable pays " // Imprime prefijo para cable turistico
              << formatCOP(student.fareOn(arvi)) << " (no flat fare there)\n\n"; // Imprime valor a pagar por estudiante en cable turistico

    cout << "============================================================\n";
    cout << "  End of demonstration (Partes 1 y 2 - Integrante 1).\n";
    cout << "============================================================\n";

    return 0;
}