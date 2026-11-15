#include "ListaPuntosGPS.h"
#include <cmath>

// Tolerancia
static const double EPS_UBICACION = 1e-6;

// ---- NodoLista ----

// Constructor por defecto
NodoLista::NodoLista() : siguiente(nullptr), puntoGPS() {
}

// Constructor sobrecargado
NodoLista::NodoLista(NodoLista* sig, const PuntoGPS& p)
    : siguiente(sig), puntoGPS(p) {
}

// Constructor de copia
NodoLista::NodoLista(const NodoLista& n)
    : siguiente(n.siguiente), puntoGPS(n.puntoGPS) {
}

// Operador de asignacion
NodoLista& NodoLista::operator=(const NodoLista& n) {
    if (this != &n) {
        siguiente = n.siguiente;
        puntoGPS = n.puntoGPS;
    }
    return *this;
}

// Destructor
NodoLista::~NodoLista() {
}

// Getters y setters
NodoLista* NodoLista::getSiguiente() const {
    return siguiente;
}

PuntoGPS NodoLista::getPuntoGPS() const {
    return puntoGPS;
}

void NodoLista::setSiguiente(NodoLista* sig) {
    siguiente = sig;
}

void NodoLista::setPuntoGPS(const PuntoGPS& p) {
    puntoGPS = p;
}

// ---- IteradorLista ----

// Constructor por defecto
IteradorLista::IteradorLista() : pt(nullptr) {
}

// Constructor de copia
IteradorLista::IteradorLista(const IteradorLista& it) : pt(it.pt) {
}

// Destructor
IteradorLista::~IteradorLista() {
}

// Operador de asignacion
IteradorLista& IteradorLista::operator=(const IteradorLista& it) {
    if (this != &it) {
        pt = it.pt;
    }
    return *this;
}

// Avanzar
void IteradorLista::step() {
    if (pt != nullptr) {
        pt = pt->getSiguiente();
    }
}

// Comparacion
bool IteradorLista::operator==(const IteradorLista& it) const {
    return pt == it.pt;
}

bool IteradorLista::operator!=(const IteradorLista& it) const {
    return pt != it.pt;
}

// ---- ListaPuntosGPS ----

// Constructor por defecto
ListaPuntosGPS::ListaPuntosGPS() : head(nullptr) {
}

// Constructor de copia
ListaPuntosGPS::ListaPuntosGPS(const ListaPuntosGPS& otra) : head(nullptr) {
    copiarDesde(otra);
}

// Operador de asignacion
ListaPuntosGPS& ListaPuntosGPS::operator=(const ListaPuntosGPS& otra) {
    if (this != &otra) {
        vaciar();
        copiarDesde(otra);
    }
    return *this;
}

// Destructor
ListaPuntosGPS::~ListaPuntosGPS() {
    vaciar();
}

// Auxiliar: vaciar
void ListaPuntosGPS::vaciar() {
    while (head != nullptr) {
        NodoLista* aux = head;
        head = head->getSiguiente();
        delete aux;
    }
}

// Auxiliar: copiar
void ListaPuntosGPS::copiarDesde(const ListaPuntosGPS& otra) {
    NodoLista* ultimo = nullptr;
    NodoLista* actual = otra.head;

    while (actual != nullptr) {
        agregarAlFinal(ultimo, actual->getPuntoGPS());
        actual = actual->getSiguiente();
    }
}

// Auxiliar: agregar al final
void ListaPuntosGPS::agregarAlFinal(NodoLista*& ultimo, const PuntoGPS& p) {
    NodoLista* nuevo = new NodoLista(nullptr, p);
    if (ultimo == nullptr) {
        head = nuevo;
    } else {
        ultimo->setSiguiente(nuevo);
    }
    ultimo = nuevo;
}

// Inicio
IteradorLista ListaPuntosGPS::begin() const {
    IteradorLista it;
    it.pt = head;
    return it;
}

// Final
IteradorLista ListaPuntosGPS::end() const {
    return IteradorLista();
}

// Obtener punto
PuntoGPS ListaPuntosGPS::getPuntoGPS(IteradorLista it) const {
    if (it.pt == nullptr) {
        return PuntoGPS();
    }
    return it.pt->getPuntoGPS();
}

// Es vacia
bool ListaPuntosGPS::esVacia() const {
    return head == nullptr;
}

// Longitud
int ListaPuntosGPS::longitud() const {
    int n = 0;
    NodoLista* actual = head;
    while (actual != nullptr) {
        n++;
        actual = actual->getSiguiente();
    }
    return n;
}

// Buscar por timestamp
IteradorLista ListaPuntosGPS::buscarPorTimestamp(unsigned long ts) const {
    NodoLista* actual = head;
    while (actual != nullptr) {
        unsigned long tsActual = actual->getPuntoGPS().getTimestamp();
        if (tsActual == ts) {
            IteradorLista it;
            it.pt = actual;
            return it;
        }
        if (tsActual > ts) {
            break;
        }
        actual = actual->getSiguiente();
    }
    return end();
}

// Existe timestamp
bool ListaPuntosGPS::existeTimestamp(unsigned long ts) const {
    return buscarPorTimestamp(ts) != end();
}

// Contar misma ubicacion
int ListaPuntosGPS::contarMismaUbicacion(const PuntoGPS& p) const {
    int n = 0;
    NodoLista* actual = head;
    while (actual != nullptr) {
        PuntoGPS q = actual->getPuntoGPS();
        if (fabs(q.getLatitud() - p.getLatitud()) < EPS_UBICACION &&
            fabs(q.getLongitud() - p.getLongitud()) < EPS_UBICACION &&
            fabs(q.getAltitud() - p.getAltitud()) < EPS_UBICACION) {
            n++;
        }
        actual = actual->getSiguiente();
    }
    return n;
}

// Insertar
bool ListaPuntosGPS::insertar(const PuntoGPS& p) {
    unsigned long ts = p.getTimestamp();
    NodoLista* anterior = nullptr;
    NodoLista* actual = head;

    while (actual != nullptr && actual->getPuntoGPS().getTimestamp() < ts) {
        anterior = actual;
        actual = actual->getSiguiente();
    }

    if (actual != nullptr && actual->getPuntoGPS().getTimestamp() == ts) {
        return false;
    }

    NodoLista* nuevo = new NodoLista(actual, p);

    if (anterior == nullptr) {
        head = nuevo;
    } else {
        anterior->setSiguiente(nuevo);
    }
    return true;
}

// Borrar primero
bool ListaPuntosGPS::borrarPrimero() {
    if (head == nullptr) {
        return false;
    }
    NodoLista* aux = head;
    head = head->getSiguiente();
    delete aux;
    return true;
}

// Borrar ultimo
bool ListaPuntosGPS::borrarUltimo() {
    if (head == nullptr) {
        return false;
    }
    if (head->getSiguiente() == nullptr) {
        delete head;
        head = nullptr;
        return true;
    }
    NodoLista* penultimo = head;
    while (penultimo->getSiguiente()->getSiguiente() != nullptr) {
        penultimo = penultimo->getSiguiente();
    }
    delete penultimo->getSiguiente();
    penultimo->setSiguiente(nullptr);
    return true;
}

// Borrar con iterador
bool ListaPuntosGPS::borrar(IteradorLista& it) {
    if (it.pt == nullptr) {
        return false;
    }

    NodoLista* anterior = nullptr;
    NodoLista* actual = head;
    while (actual != nullptr && actual != it.pt) {
        anterior = actual;
        actual = actual->getSiguiente();
    }

    if (actual == nullptr) {
        return false;
    }

    if (anterior == nullptr) {
        head = actual->getSiguiente();
    } else {
        anterior->setSiguiente(actual->getSiguiente());
    }
    delete actual;
    it.pt = nullptr;
    return true;
}

// Borrar por timestamp
bool ListaPuntosGPS::borrarPorTimestamp(unsigned long ts) {
    IteradorLista it = buscarPorTimestamp(ts);
    return borrar(it);
}

// Asignar
bool ListaPuntosGPS::asignar(IteradorLista it, const PuntoGPS& p) {
    if (it.pt == nullptr) {
        return false;
    }

    NodoLista* anterior = nullptr;
    NodoLista* actual = head;
    while (actual != nullptr && actual != it.pt) {
        anterior = actual;
        actual = actual->getSiguiente();
    }
    if (actual == nullptr) {
        return false;
    }

    unsigned long ts = p.getTimestamp();

    if (anterior != nullptr && anterior->getPuntoGPS().getTimestamp() >= ts) {
        return false;
    }
    NodoLista* siguiente = actual->getSiguiente();
    if (siguiente != nullptr && siguiente->getPuntoGPS().getTimestamp() <= ts) {
        return false;
    }

    actual->setPuntoGPS(p);
    return true;
}

// Filtrar por rango
ListaPuntosGPS ListaPuntosGPS::filtrarRangoTimestamp(unsigned long inicio,
                                                     unsigned long fin) const {
    ListaPuntosGPS resultado;
    NodoLista* ultimo = nullptr;
    NodoLista* actual = head;

    while (actual != nullptr) {
        PuntoGPS p = actual->getPuntoGPS();
        if (p.getTimestamp() >= inicio && p.getTimestamp() <= fin) {
            resultado.agregarAlFinal(ultimo, p);
        }
        actual = actual->getSiguiente();
    }
    return resultado;
}

// Filtrar por dispositivo
ListaPuntosGPS ListaPuntosGPS::filtrarDispositivo(const string& dispositivo) const {
    ListaPuntosGPS resultado;
    NodoLista* ultimo = nullptr;
    NodoLista* actual = head;

    while (actual != nullptr) {
        PuntoGPS p = actual->getPuntoGPS();
        if (p.getDispositivo() == dispositivo) {
            resultado.agregarAlFinal(ultimo, p);
        }
        actual = actual->getSiguiente();
    }
    return resultado;
}

// Distancia total
double ListaPuntosGPS::distanciaTotal() const {
    if (head == nullptr || head->getSiguiente() == nullptr) {
        return 0.0;
    }

    double total = 0.0;
    NodoLista* actual = head;
    while (actual->getSiguiente() != nullptr) {
        PuntoGPS a = actual->getPuntoGPS();
        PuntoGPS b = actual->getSiguiente()->getPuntoGPS();
        total += a.distancia3D(b);
        actual = actual->getSiguiente();
    }
    return total;
}

// Velocidad media
double ListaPuntosGPS::velocidadMedia() const {
    if (head == nullptr || head->getSiguiente() == nullptr) {
        return 0.0;
    }

    NodoLista* ultimo = head;
    while (ultimo->getSiguiente() != nullptr) {
        ultimo = ultimo->getSiguiente();
    }

    unsigned long tPrimero = head->getPuntoGPS().getTimestamp();
    unsigned long tUltimo = ultimo->getPuntoGPS().getTimestamp();
    if (tUltimo <= tPrimero) {
        return 0.0;
    }

    return distanciaTotal() / static_cast<double>(tUltimo - tPrimero);
}

// Velocidad maxima
double ListaPuntosGPS::velocidadMaxima() const {
    if (head == nullptr || head->getSiguiente() == nullptr) {
        return 0.0;
    }

    double maxima = head->getPuntoGPS().velocidad(head->getSiguiente()->getPuntoGPS());

    NodoLista* actual = head->getSiguiente();
    while (actual->getSiguiente() != nullptr) {
        PuntoGPS a = actual->getPuntoGPS();
        PuntoGPS b = actual->getSiguiente()->getPuntoGPS();
        double v = a.velocidad(b);
        if (v > maxima) {
            maxima = v;
        }
        actual = actual->getSiguiente();
    }
    return maxima;
}

// A cadena
string ListaPuntosGPS::aCadena() const {
    string resultado;
    NodoLista* actual = head;
    while (actual != nullptr) {
        if (actual != head) {
            resultado += "\n";
        }
        resultado += actual->getPuntoGPS().aCadena();
        actual = actual->getSiguiente();
    }
    return resultado;
}
