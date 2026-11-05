#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

// ESTRUCTURAS Y FUNCIONES BASE DEL SISTEMA

string formatCOP(long long value) {
    string s = to_string(value);
    int n = static_cast<int>(s.length());
    for (int i = n - 3; i > 0; i -= 3) {
        s.insert(i, ".");
    }
    return "$ " + s;
}

class TransportMode;

struct Leg {
    int minute;
    const TransportMode* mode;
};

struct ChargedLeg {
    int minute;
    string modeName;
    string serviceType;
    long long charge;
    bool integrated;
};

// PARTE 1: JERARQUIA DE TRANSPORTMODE

class TransportMode {
protected:
    string nombreModo; // Nombre descriptivo o identificador de la linea
    int numeroEstaciones; // Cantidad total de estaciones del recorrido

public:
    TransportMode(const string& nombre, int estaciones)
        : nombreModo(nombre), numeroEstaciones(estaciones) {} // Asigna atributos principales del modo

    virtual ~TransportMode() = default; // Destructor virtual para garantizaz eliminacion destruccion limpia en derivadas

    virtual long long baseFare() const = 0; // Metodo virtual puro para obtener la tarifa plena base
    virtual string serviceType() const = 0; // Metodo virtual puro que identifica el tipo de servicio

    virtual bool allowsStudentFlatFare() const { return true; } // Hook que admite tarifa plana de estudiante por defecto
    virtual bool allowsIntegration() const { return true; } // Hook que autoriza la integracion tarifaria por defecto
    virtual bool opensIntegrationWindow() const { return true; } // Hook que permite abrir ventana de 90 minutos por defecto

    string describe() const {
        return serviceType() + " " + nombreModo + " (" + to_string(numeroEstaciones) + " stations) full fare " + formatCOP(baseFare()); // Construye la descripcion en el formato estandar
    }

    string getNombreModo() const { return nombreModo; } // Retorna el nombre del modo
    int getNumeroEstaciones() const { return numeroEstaciones; } // Retorna la cantidad de estaciones
};

class MetroLine : public TransportMode {
public:
    MetroLine(const string& nombre, int estaciones)
        : TransportMode(nombre, estaciones) {} // Transfiere datos iniciales a TransportMode

    long long baseFare() const override { return 3820LL; } // Ocurrencia 1 de 3 del literal 3820
    string serviceType() const override { return "Metro line"; } // Retorna identificador de servicio de metro
};

class TramLine : public TransportMode {
public:
    TramLine(const string& nombre, int estaciones)
        : TransportMode(nombre, estaciones) {} // Transfiere datos iniciales a TransportMode

    long long baseFare() const override { return 3820LL; } // Ocurrencia 2 de 3 del literal 3820
    string serviceType() const override { return "Tram line"; } // Retorna identificador de servicio de tranvia
};

class CableLine : public TransportMode {
public:
    CableLine(const string& nombre, int estaciones)
        : TransportMode(nombre, estaciones) {} // Transfiere datos iniciales a TransportMode

    long long baseFare() const override { return 3820LL; } // Ocurrencia 3 de 3 del literal 3820
    string serviceType() const override { return "Cable line"; } // Retorna identificador de servicio de metrocable
};

class TouristCable : public CableLine {
private:
    long long recargo; // Monto adicional especifico para la tarifa turistica

public:
    TouristCable(const string& nombre, int estaciones, long long valorRecargo)
        : CableLine(nombre, estaciones), recargo(valorRecargo) {} // Inicializa superclase y asigna el recargo

    long long baseFare() const override { return CableLine::baseFare() + recargo; } // Reutiliza tarifa de CableLine sin usar el literal 3820
    string serviceType() const override { return "Tourist cable"; } // Retorna identificador de servicio de cable turistico

    bool allowsStudentFlatFare() const override { return false; } // Cancela la tarifa plana de estudiante
    bool allowsIntegration() const override { return false; } // Cancela la integracion tarifaria
    bool opensIntegrationWindow() const override { return false; } // Cancela la apertura de ventana de integracion
};

class FeederRoute : public TransportMode {
public:
    FeederRoute(const string& nombre, int estaciones)
        : TransportMode(nombre, estaciones) {} // Transfiere datos iniciales a TransportMode

    long long baseFare() const override { return 0LL; } // Tarifa cero por estar incluida en la tarifa del sistema
    string serviceType() const override { return "Feeder route"; } // Retorna identificador de ruta alimentadora

    bool opensIntegrationWindow() const override { return false; } // Evita que una ruta alimentadora inicie ventana de integracion
};

// PARTE 2: JERARQUIA DE PASSENGER (PATRÓN NON-VIRTUAL INTERFACE)

class Passenger {
public:
    Passenger() = default; // Constructor por defecto
    virtual ~Passenger() = default; // Destructor virtual por defecto

    long long fareOn(const TransportMode& mode) const {
        return max(0LL, computeFare(mode)); // Punto de entrada NVI que impide tarifas finales negativas
    }

protected:
    virtual long long computeFare(const TransportMode& mode) const {
        return mode.baseFare(); // Comportamiento por defecto: retorna la tarifa plena del modo
    }
};

class FrequentPassenger : public Passenger {
public:
    FrequentPassenger() = default; // Hereda directamente la tarifa plena
    virtual ~FrequentPassenger() = default; // Destructor por defecto
};

class StudentPassenger : public Passenger {
public:
    StudentPassenger() = default; // Constructor por defecto
    virtual ~StudentPassenger() = default; // Destructor por defecto

protected:
    long long computeFare(const TransportMode& mode) const override {
        if (mode.allowsStudentFlatFare()) { // Consulta polimorfica de la politica del modo
            return 1600LL; // Aplica la tarifa plana fija para estudiante
        }
        return Passenger::computeFare(mode); // Recurre a la tarifa plena si el modo no lo permite
    }
};

class SeniorPassenger : public Passenger {
public:
    SeniorPassenger() = default; // Constructor por defecto
    virtual ~SeniorPassenger() = default; // Destructor por defecto

protected:
    long long computeFare(const TransportMode& mode) const override {
        return Passenger::computeFare(mode) - 490LL; // Descuenta 490 pesos a la tarifa base
    }
};

class DisabledPassenger : public Passenger {
public:
    DisabledPassenger() = default; // Constructor por defecto
    virtual ~DisabledPassenger() = default; // Destructor por defecto

protected:
    long long computeFare(const TransportMode& mode) const override {
        return Passenger::computeFare(mode) - 1100LL; // Descuenta 1100 pesos a la tarifa base
    }
};

class OccasionalPassenger : public Passenger {
public:
    OccasionalPassenger() = default; // Constructor por defecto
    virtual ~OccasionalPassenger() = default; // Destructor por defecto

protected:
    long long computeFare(const TransportMode& mode) const override {
        return Passenger::computeFare(mode) + 580LL; // Agrega 580 pesos de recargo a la tarifa base
    }
};

// MAIN DE PRUEBAS

int main() {
    cout << "INTEGRATED ITINERARY AND FARE ENGINE Valle de Aburra\n"; // Imprime encabezado del programa
    cout << "====================================================\n\n";

    // Instanciacion de los modos de transporte requeridos
    MetroLine mA("A", 21); // Instancia Metro Linea A
    MetroLine mB("B", 7); // Instancia Metro Linea B
    TramLine tA("T-A Ayacucho", 9); // Instancia Tranvia Ayacucho
    CableLine cK("K Santo Domingo", 4); // Instancia Metrocable Linea K
    TouristCable cL("L Arvi", 2, 9180LL); // Instancia Cable Arvi con recargo
    FeederRoute r130("130 Robledo", 18); // Instancia Ruta Alimentadora 130

    // 1. Demostracion del inventario polimorfico
    cout << ">> 1. NETWORK INVENTORY (polymorphic describe)\n";
    cout << "- " << mA.describe() << "\n"; // Imprime descripcion de Metro A
    cout << "- " << mB.describe() << "\n"; // Imprime descripcion de Metro B
    cout << "- " << tA.describe() << "\n"; // Imprime descripcion de Tranvia
    cout << "- " << cK.describe() << "\n"; // Imprime descripcion de Cable K
    cout << "- " << cL.describe() << "\n"; // Imprime descripcion de Cable Arvi
    cout << "- " << r130.describe() << "\n\n"; // Imprime descripcion de Alimentador

    // 2. Demostracion de tarifas por perfil sobre Metro Linea A y Cable Turistico
    cout << ">> 2. FULL FARE PER PROFILE ON METRO LINE A\n";
    FrequentPassenger freq; // Instancia perfil frecuente
    StudentPassenger stud; // Instancia perfil estudiante
    SeniorPassenger sen; // Instancia perfil adulto mayor

    cout << "Civica frequent pays " << formatCOP(freq.fareOn(mA)) << "\n"; // Muestra cobro a pasajero frecuente
    cout << "Municipal student pays " << formatCOP(stud.fareOn(mA)) << "\n"; // Muestra cobro a estudiante en metro
    cout << "Senior citizen pays " << formatCOP(sen.fareOn(mA)) << "\n"; // Muestra cobro a adulto mayor en metro
    cout << "Municipal student on the tourist cable pays " << formatCOP(stud.fareOn(cL)) << " (no flat fare there)\n\n"; // Muestra cobro a estudiante en cable turistico

    cout << "====================================================\n";
    cout << "Pruebas de Partes 1 y 2 ejecutadas con exito.\n";
    cout << "====================================================\n";

    return 0; // Retorna 0 indicando ejecucion exitosa
}