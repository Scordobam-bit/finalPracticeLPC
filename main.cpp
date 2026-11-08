/* =========================================================================
    Write here the full name of the members of the team formed for the
    development of the practice

    Juan Camilo Gómez
    Samuel Córdoba
    =========================================================================

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
std::string formatCOP(long long value) {
	std::string digits = std::to_string(value);
	std::string out;
	int count = 0;
	for (auto it = digits.rbegin(); it != digits.rend(); ++it) {
		out.push_back(*it);
		if (++count % 3 == 0 && it + 1 != digits.rend()) {
			out.push_back('.');
		}
	}
	std::reverse(out.begin(), out.end());
	return "$ " + out;
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

    // TODO 1.1  Declare baseFare() and serviceType() as PURE VIRTUAL.
	//           This class cannot know either value: that is the whole point.

    virtual long long baseFare() const = 0; // Metodo virtual puro para obtener la tarifa basica
    virtual string serviceType() const = 0; // Metodo virtual puro para obtener el tipo de servicio

	// TODO 1.2  Declare the three hooks with a sensible default of true:
	//           allowsStudentFlatFare(), allowsIntegration(),
	//           opensIntegrationWindow().

    virtual bool allowsStudentFlatFare() const { return true; } // Hook virtual que permite tarifa plana de estudiante por defecto
    virtual bool allowsIntegration() const { return true; } // Hook virtual que autoriza integracion tarifaria por defecto
    virtual bool opensIntegrationWindow() const { return true; } // Hook virtual que autoriza abrir ventana de integracion por defecto

	// TODO 1.3  Implement describe() as a NON-virtual template method that
	//           returns, for example:
	//              "Metro line A (21 stations) - full fare $ 3.820"
	//           It must call the pure virtual members through this object.

    string describe() const { // Metodo no virtual que genera la descripcion formateada del modo
        return serviceType() + " " + name_ + " (" + to_string(stations_) + " stations) - full fare " + formatCOP(baseFare()); // Construye y retorna la cadena descriptiva
    }

private:
    string name_; // Atributo privado con el nombre del modo
    int stations_; // Atributo privado con la cantidad de estaciones
};

// TODO 1.4  Write the five concrete modes. Full fare of the integrated
//           system in 2026: 3820 pesos.
//
//              MetroLine      serviceType "Metro line"    fare 3820
//              TramLine       serviceType "Tram line"     fare 3820
//              CableLine      serviceType "Cable line"    fare 3820
//              TouristCable   inherits from CableLine, adds a surcharge
//                             given in its constructor. It must NOT repeat
//                             the 3820 literal: obtain the parent value with
//                             CableLine::baseFare(). No student flat fare,
//                             no integration, does not open a window.
//              FeederRoute    serviceType "Feeder route"  fare 0
//                             Free because the trunk fare covers it, so it
//                             must NOT open an integration window.

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

    bool opensIntegrationWindow() const override { return false; } // Deniega la apertura de ventana de integracion para rutas alimentadoras
};

/* -------------------------------------------------------------------------
    PART 2. Passenger hierarchy, Non-Virtual Interface idiom
    fareOn() is NOT virtual: it is the template method that clamps the result.
    computeFare() is the customisation point each profile overrides.
   ------------------------------------------------------------------------- */
// TODO 2.1  Write the Passenger hierarchy using the Non-Virtual Interface
//           idiom, which is the C++ way of writing a template method:
//
//              public:    long long fareOn(const TransportMode&) const
//                         NOT virtual. It calls computeFare and clamps the
//                         result with std::max(0LL, ...), because a fare is
//                         never negative.
//              protected: virtual long long computeFare(const TransportMode&) const
//                         Default behaviour: the full fare of the mode.
//
//           Also declare profile() as pure virtual and give the class a
//           virtual destructor.
//

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

// TODO 2.2  Write the five profiles. Each one overrides computeFare and
//           obtains the full fare by calling Passenger::computeFare(mode),
//           which is the C++ equivalent of 'super'. Civica fares, 2026:
//
//              FrequentPassenger      "Civica frequent"         3820
//                                     does NOT override computeFare
//              StudentPassenger       "Municipal student"       1600 flat,
//                                     but only when the mode allows it
//              SeniorPassenger        "Senior citizen"          base - 490
//              DisabilityPassenger    "Person with disability"  base - 1100
//              BearerPassenger        "Bearer (occasional)"     base + 580

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
    PART 3. The itinerary and the integration rule
   ------------------------------------------------------------------------- */
struct Leg {
    int minute;                   // minutes elapsed since the journey started
    const TransportMode* mode;    // non-owning observer pointer
};

struct ChargedLeg {
    int minute;
    std::string modeName;
    std::string serviceType;
    long long charge;
    bool integrated;              // true: free transfer inside the window
};
// TODO 3.1  Write the Network class. It OWNS its modes:
//
//              std::vector<std::unique_ptr<TransportMode>> modes_;
//              TransportMode& add(std::unique_ptr<TransportMode> mode);
//              const std::vector<std::unique_ptr<TransportMode>>& modes() const;
//              void printInventory() const;   // one line per mode
//
//           add() returns a reference to the stored object so that main()
//           can keep observing it without owning it.

class Network { // Clase que posee y administra todos los medios de transporte de la red.
public:

    // Recibe un medio de transporte, transfiere su propiedad a la red
    // y devuelve una referencia para que main() pueda observarlo.
    TransportMode& add(unique_ptr<TransportMode> mode) {     
        modes_.push_back(move(mode));                        
        return *modes_.back();
    }

    // Permite consultar los medios almacenados sin modificarlos.
    const vector<unique_ptr<TransportMode>>& modes() const { 
        return modes_;
    }

    // Imprime una descripción de cada medio de transporte de la red.
    void printInventory() const {    
        for_each(modes_.begin(), modes_.end(),
        [](const unique_ptr<TransportMode>& mode) {
            cout << "  - " << mode -> describe() << '\n';
        });
    }
private:
    // Lista que posee los medios de transporte mediante punteros inteligentes.
    vector<unique_ptr<TransportMode>> modes_;  
};

// TODO 3.2  Write chargeItinerary, which applies the integration rule:
//

//    std::vector<ChargedLeg> chargeItinerary(std::vector<Leg> legs,
//                                            const Passenger& passenger);
//
//      a) sort the legs by minute with std::sort and a lambda comparator;
//         the itinerary in main() is deliberately out of order
//      b) the first validation pays the full fare and opens a 90-minute
//         window
//      c) up to TWO transfers inside the window are free
//      d) a mode with allowsIntegration() == false always pays and never
//         touches the window
//      e) a mode with opensIntegrationWindow() == false pays its own fare
//         but leaves the window untouched
//      f) when the window expires or the two free transfers run out, the
//         next validation pays again and opens a new window
//
//    This is the only place where an explicit loop over the legs is allowed.

// Ordena el itinerario y calcula el cobro de cada tramo según las reglas
// de integración y el perfil del pasajero.
vector<ChargedLeg> chargeItinerary(
    vector<Leg> legs
    , const Passenger& passenger) {

    // Ordena los tramos por el minuto en que ocurren.
	sort(legs.begin(), legs.end(),
	[](const Leg& first, const Leg& second) {
		return first.minute < second.minute;
	});

    // Guardará los tramos con sus cobros ya calculados.
	vector<ChargedLeg> charged;

    // Estado de la ventana de integración.
	bool windowOpen = false;
	int windowStart = 0;
	int freeTransfers = 0;

    // Recorre los tramos ordenados. Este es el único ciclo explícito permitido.
	for (const Leg& leg : legs) {

        // Obtiene el medio de transporte observado por este tramo.
		const TransportMode& mode = *leg.mode;

        // Valores iniciales del resultado para el tramo actual.
		long long charge = 0;
		bool integrated = false;

        // Un modo que no permite integración, como TouristCable,
        // siempre cobra y no modifica la ventana.
		if (!mode.allowsIntegration()) {
			charge = passenger.fareOn(mode);

        // Un modo que no abre ventana, como FeederRoute,
        // paga su propia tarifa y deja la ventana sin cambios.
		} else if (!mode.opensIntegrationWindow()) {
			charge = passenger.fareOn(mode);

        // Si la ventana está vigente y quedan transbordos gratuitos,
        // este tramo no tiene cobro.
		} else if (windowOpen && leg.minute <= windowStart + 90 && freeTransfers < 2) {
			integrated = true;
			++freeTransfers;

        // Si no hay ventana, expiró o se agotaron los dos transbordos,
        // se cobra la tarifa y se abre una nueva ventana.
		} else {
			charge = passenger.fareOn(mode);
			windowOpen = true;
			windowStart = leg.minute;
			freeTransfers = 0;
		}

        // Guarda el resultado calculado para este tramo.
		charged.push_back({
			leg.minute,
			mode.name(),
			mode.serviceType(),
			charge,
			integrated
		});
	}
    // Devuelve todo el itinerario con sus cobros calculados.
	return charged;
}

/* -------------------------------------------------------------------------
    PART 4. Reports built exclusively with STL algorithms and lambdas
   ------------------------------------------------------------------------- */
// TODO 4.1  long long totalPaid(const std::vector<ChargedLeg>&)
//           Total paid. Use std::accumulate with a lambda. No loops.

// Suma todos los cobros realizados durante el itinerario.
long long totalPaid(const vector<ChargedLeg>& charged) {
	return accumulate(
	    charged.begin(),
	    charged.end(),
	    0LL,

        // Suma el cobro del tramo actual al total acumulado.   
	    [](long long total, const ChargedLeg& leg) {
		return total + leg.charge;
	});
}

//
// TODO 4.2  long long freeTransferCount(const std::vector<ChargedLeg>&)
//           Number of free transfers. Use std::count_if. No loops.
//

// Cuenta cuántos tramos fueron transbordos gratuitos.
long long freeTransferCount(const std::vector<ChargedLeg>& charged) {
	return count_if(
	    charged.begin(),
	    charged.end(),

        // Cuenta únicamente los tramos marcados como integrados.
	    [](const ChargedLeg& leg) {
		return leg.integrated;
	});
}

// TODO 4.3  std::map<std::string, long long>
//               paidByServiceType(const std::vector<ChargedLeg>&)
//           Amount paid per service type. Use std::for_each with a lambda
//           that captures the map by reference. No loops.
//

// Calcula el total pagado por cada tipo de servicio.
map<string, long long> paidByServiceType(const std::vector<ChargedLeg>& charged) {
	map<string, long long> totals;

	for_each(
	    charged.begin(),
	    charged.end(),

        // Acumula el cobro usando el tipo de servicio como clave.
	    [&totals](const ChargedLeg& leg) {
	    totals[leg.serviceType] += leg.charge;
	});
	return totals;
}

// TODO 4.4  std::vector<std::string> namesMatching(
//               const Network&,
//               const std::function<bool(const TransportMode&)>& policy)
//           Names of the modes that satisfy a criterion the CALLER injects.
//           The network must not know what the criterion is.
//

// Devuelve los nombres de los medios que cumplen la condición recibida.
vector<string> namesMatching(
    const Network& network,
    const function<bool(const TransportMode&)>& policy) {
	vector<string> names;

	for_each(
	    network.modes().begin(),
	    network.modes().end(),

        // Si el modo cumple la política, agrega su nombre al resultado.
    	[&names, &policy](const unique_ptr<TransportMode>& mode) {
		if (policy(*mode)) {
			names.push_back(mode -> name());
		}
	});
	return names;

}

// TODO 4.5  std::string joined(const std::vector<std::string>&)
//           Join with ", ". Use std::accumulate over strings. No loops.

// Une los nombres de una lista separándolos con ", ".
string joined(const vector<string>& names) {
	return accumulate(
	    names.begin(),
	    names.end(),
	    string{},

        // El primer nombre se agrega sin coma; los siguientes llevan ", " antes.
    	[](const string& result, const string& name) {
		return result.empty() ? name : result + ", " + name;
	});
}

/* -------------------------------------------------------------------------
    MAIN
   ------------------------------------------------------------------------- */
int main() {
	std::cout << "============================================================\n";
	std::cout << "   INTEGRATED ITINERARY AND FARE ENGINE - Valle de Aburra\n";
	std::cout << "============================================================\n\n";

	Network network;
	TransportMode& lineA  = network.add(std::make_unique<MetroLine>("A", 21));
	TransportMode& lineB  = network.add(std::make_unique<MetroLine>("B", 7));
	TransportMode& tramTA = network.add(std::make_unique<TramLine>("T-A Ayacucho", 9));
	TransportMode& cableK = network.add(std::make_unique<CableLine>("K Santo Domingo", 4));
	TransportMode& arvi   = network.add(std::make_unique<TouristCable>("L Arvi", 2, 9180));
	TransportMode& feeder = network.add(std::make_unique<FeederRoute>("130 Robledo", 18));

	std::cout << ">> 1. NETWORK INVENTORY (polymorphic describe)\n";
	network.printInventory();
	std::cout << '\n';

	/* Itinerary deliberately given out of chronological order, so that the
	   sort step is observable. */
	const std::vector<Leg> itinerary = {
		{45,  &cableK},
		{0,   &feeder},
		{12,  &lineA},
		{150, &lineB},
		{70,  &tramTA},
		{200, &arvi}
	};

	const FrequentPassenger frequent("Diego");
	const StudentPassenger  student("Valentina");
	const SeniorPassenger   senior("Dona Amparo");

	std::cout << ">> 2. FULL FARE PER PROFILE ON METRO LINE A\n";
	std::cout << "  " << frequent.profile() << " pays " << formatCOP(frequent.fareOn(lineA)) << '\n';
	std::cout << "  " << student.profile()  << " pays " << formatCOP(student.fareOn(lineA))  << '\n';
	std::cout << "  " << senior.profile()   << " pays " << formatCOP(senior.fareOn(lineA))   << '\n';
	std::cout << "  " << student.profile()  << " on the tourist cable pays "
	          << formatCOP(student.fareOn(arvi)) << " (no flat fare there)\n\n";

	const std::vector<ChargedLeg> charged = chargeItinerary(itinerary, frequent);

	std::cout << ">> 3. CHARGED ITINERARY (frequent profile)\n";
	std::for_each(charged.begin(), charged.end(), [](const ChargedLeg& c) {
		std::cout << "  min " << c.minute << " | " << c.serviceType << " " << c.modeName
		          << " | " << formatCOP(c.charge)
		          << (c.integrated ? "  [free transfer]" : "") << '\n';
	});
	std::cout << '\n';

	std::cout << ">> 4. REPORTS BUILT WITH STL ALGORITHMS\n";
	std::cout << "  accumulate: total paid -> " << formatCOP(totalPaid(charged)) << '\n';
	std::cout << "  count_if: free transfers -> " << freeTransferCount(charged) << '\n';

	const auto byType = paidByServiceType(charged);
	std::for_each(byType.begin(), byType.end(),
	[](const std::pair<const std::string, long long>& entry) {
		std::cout << "  map: paid on " << entry.first << " -> "
		          << formatCOP(entry.second) << '\n';
	});

	const auto firstPaid = std::find_if(charged.begin(), charged.end(),
	[](const ChargedLeg& c) {
		return c.charge > 0;
	});
	std::cout << "  find_if: first leg that actually paid -> "
	          << (firstPaid == charged.end() ? std::string("none") : firstPaid->modeName) << '\n';

	const std::function<bool(const TransportMode&)> bigModes =
	[](const TransportMode& m) {
		return m.stations() > 8;
	};
	std::cout << "  injected policy: modes with more than 8 stations -> "
	          << joined(namesMatching(network, bigModes)) << '\n';

	std::cout << "  total for the student on the same itinerary -> "
	          << formatCOP(totalPaid(chargeItinerary(itinerary, student))) << '\n';
	std::cout << "  total for the senior on the same itinerary -> "
	          << formatCOP(totalPaid(chargeItinerary(itinerary, senior))) << '\n';

	std::cout << "\n============================================================\n";
	std::cout << "  End of demonstration.\n";
	std::cout << "============================================================\n";
	return 0;
}