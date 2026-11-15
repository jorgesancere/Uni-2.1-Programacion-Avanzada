#include "ListaPuntosGPS.h"
#include <cmath>

// Tolerancia para comparar ubicaciones (igual que operator== de PuntoGPS)
static const double EPS_UBICACION = 1e-6;

// ===============================================================
// NodoLista
// NUNCA se reserva ni libera memoria de nodos adyacentes aquí.
// ===============================================================

NodoLista::NodoLista() : siguiente(nullptr), puntoGPS() {
}

NodoLista::NodoLista(NodoLista* sig, const PuntoGPS& p)
    : siguiente(sig), puntoGPS(p) {
}

// Copia superficial del puntero: sin new
NodoLista::NodoLista(const NodoLista& n)
    : siguiente(n.siguiente), puntoGPS(n.puntoGPS) {
}

NodoLista& NodoLista::operator=(const NodoLista& n) {
    if (this != &n) {
        siguiente = n.siguiente;
        puntoGPS = n.puntoGPS;
    }
    return *this;
}

// Vacío a propósito: un delete siguiente borraría la lista en cascada
NodoLista::~NodoLista() {
}

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

// ===============================================================
// IteradorLista
// ===============================================================

IteradorLista::IteradorLista() : pt(nullptr) {
}

IteradorLista::IteradorLista(const IteradorLista& it) : pt(it.pt) {
}

IteradorLista::~IteradorLista() {
}

IteradorLista& IteradorLista::operator=(const IteradorLista& it) {
    if (this != &it) {
        pt = it.pt;
    }
    return *this;
}

void IteradorLista::step() {
    if (pt != nullptr) {
        pt = pt->getSiguiente();
    }
}

bool IteradorLista::operator==(const IteradorLista& it) const {
    return pt == it.pt;
}

bool IteradorLista::operator!=(const IteradorLista& it) const {
    return pt != it.pt;
}

// ===============================================================
// ListaPuntosGPS: forma canónica (Sesión 3)
// ===============================================================

ListaPuntosGPS::ListaPuntosGPS() : head(nullptr) {
}

// Copia profunda: un nodo nuevo por cada nodo de la otra lista
ListaPuntosGPS::ListaPuntosGPS(const ListaPuntosGPS& otra) : head(nullptr) {
    copiarDesde(otra);
}

ListaPuntosGPS& ListaPuntosGPS::operator=(const ListaPuntosGPS& otra) {
    if (this != &otra) {
        vaciar();            // 1º liberar lo que había
        copiarDesde(otra);   // 2º copiar lo nuevo
    }
    return *this;
}

ListaPuntosGPS::~ListaPuntosGPS() {
    vaciar();
}

// ---- auxiliares privados ----

// Libera todos los nodos y deja la lista vacía
void ListaPuntosGPS::vaciar() {
    while (head != nullptr) {
        NodoLista* aux = head;          // guardo el nodo a borrar
        head = head->getSiguiente();    // avanzo ANTES de borrar
        delete aux;
    }
}

// Supone que la lista está vacía (head == nullptr)
void ListaPuntosGPS::copiarDesde(const ListaPuntosGPS& otra) {
    NodoLista* ultimo = nullptr;   // última posición de MI lista (cola)
    NodoLista* actual = otra.head;

    while (actual != nullptr) {
        agregarAlFinal(ultimo, actual->getPuntoGPS());
        actual = actual->getSiguiente();
    }
}

// Añade un nodo nuevo tras 'ultimo' (la cola de ESTA lista, o nullptr si
// está vacía) y actualiza 'ultimo'. Se usa al construir listas ya ordenadas
// (copia y filtros) para no recorrer la lista en cada inserción.
void ListaPuntosGPS::agregarAlFinal(NodoLista*& ultimo, const PuntoGPS& p) {
    NodoLista* nuevo = new NodoLista(nullptr, p);
    if (ultimo == nullptr) {
        head = nuevo;                 // primer nodo
    } else {
        ultimo->setSiguiente(nuevo);  // engancho al final
    }
    ultimo = nuevo;
}

// ===============================================================
// ListaPuntosGPS: iteración y consulta básica (Sesión 1)
// ===============================================================

IteradorLista ListaPuntosGPS::begin() const {
    IteradorLista it;
    it.pt = head;
    return it;
}

IteradorLista ListaPuntosGPS::end() const {
    return IteradorLista();   // pt = nullptr
}

PuntoGPS ListaPuntosGPS::getPuntoGPS(IteradorLista it) const {
    if (it.pt == nullptr) {
        return PuntoGPS();
    }
    return it.pt->getPuntoGPS();
}

bool ListaPuntosGPS::esVacia() const {
    return head == nullptr;
}

int ListaPuntosGPS::longitud() const {
    int n = 0;
    NodoLista* actual = head;
    while (actual != nullptr) {
        n++;
        actual = actual->getSiguiente();
    }
    return n;
}

// ===============================================================
// ListaPuntosGPS: búsqueda y consulta (Sesión 2)
// ===============================================================

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
            break;   // lista ordenada: ya no puede aparecer después
        }
        actual = actual->getSiguiente();
    }
    return end();
}

bool ListaPuntosGPS::existeTimestamp(unsigned long ts) const {
    return buscarPorTimestamp(ts) != end();
}

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

// ===============================================================
// ListaPuntosGPS: inserción ordenada (Sesión 2)
// ===============================================================

bool ListaPuntosGPS::insertar(const PuntoGPS& p) {
    unsigned long ts = p.getTimestamp();
    NodoLista* anterior = nullptr;
    NodoLista* actual = head;

    // Avanzo mientras el nodo actual sea ANTERIOR en el tiempo al nuevo
    // (el orden de las condiciones importa: primero comprobar nullptr)
    while (actual != nullptr && actual->getPuntoGPS().getTimestamp() < ts) {
        anterior = actual;
        actual = actual->getSiguiente();
    }

    // Duplicado: se comprueba ANTES de reservar memoria
    if (actual != nullptr && actual->getPuntoGPS().getTimestamp() == ts) {
        return false;
    }

    NodoLista* nuevo = new NodoLista(actual, p);

    if (anterior == nullptr) {
        head = nuevo;                    // va el primero (o lista vacía)
    } else {
        anterior->setSiguiente(nuevo);   // va en medio o al final
    }
    return true;
}

// ===============================================================
// ListaPuntosGPS: borrado (Sesión 3)
// ===============================================================

bool ListaPuntosGPS::borrarPrimero() {
    if (head == nullptr) {
        return false;
    }
    NodoLista* aux = head;
    head = head->getSiguiente();
    delete aux;
    return true;
}

bool ListaPuntosGPS::borrarUltimo() {
    if (head == nullptr) {
        return false;
    }
    if (head->getSiguiente() == nullptr) {   // un único nodo
        delete head;
        head = nullptr;
        return true;
    }
    // Me paro en el PENÚLTIMO nodo
    NodoLista* penultimo = head;
    while (penultimo->getSiguiente()->getSiguiente() != nullptr) {
        penultimo = penultimo->getSiguiente();
    }
    delete penultimo->getSiguiente();
    penultimo->setSiguiente(nullptr);
    return true;
}

bool ListaPuntosGPS::borrar(IteradorLista& it) {
    if (it.pt == nullptr) {
        return false;
    }

    // Busco el nodo y su anterior (lista simplemente enlazada)
    NodoLista* anterior = nullptr;
    NodoLista* actual = head;
    while (actual != nullptr && actual != it.pt) {
        anterior = actual;
        actual = actual->getSiguiente();
    }

    if (actual == nullptr) {
        return false;   // el iterador no pertenece a esta lista
    }

    if (anterior == nullptr) {
        head = actual->getSiguiente();               // borro el primero
    } else {
        anterior->setSiguiente(actual->getSiguiente());   // salto el nodo
    }
    delete actual;
    it.pt = nullptr;   // el enunciado lo exige
    return true;
}

bool ListaPuntosGPS::borrarPorTimestamp(unsigned long ts) {
    IteradorLista it = buscarPorTimestamp(ts);
    return borrar(it);   // si it == end() (nullptr), borrar devuelve false
}

// ===============================================================
// ListaPuntosGPS: modificación de un elemento (Sesión 4)
// ===============================================================

bool ListaPuntosGPS::asignar(IteradorLista it, const PuntoGPS& p) {
    if (it.pt == nullptr) {
        return false;
    }

    // Localizo el nodo y su anterior (también comprueba que el iterador
    // pertenece a esta lista)
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

    // Orden estricto respecto al anterior y al siguiente. Como la lista ya
    // está estrictamente ordenada, esto también garantiza que no hay duplicados.
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

// ===============================================================
// ListaPuntosGPS: filtrado (Sesión 4)
// ===============================================================

ListaPuntosGPS ListaPuntosGPS::filtrarRangoTimestamp(unsigned long inicio,
                                                     unsigned long fin) const {
    ListaPuntosGPS resultado;
    NodoLista* ultimo = nullptr;   // cola de 'resultado'
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

// ===============================================================
// ListaPuntosGPS: cálculos sobre la trayectoria (Sesión 4)
// ===============================================================

double ListaPuntosGPS::distanciaTotal() const {
    if (head == nullptr || head->getSiguiente() == nullptr) {
        return 0.0;   // menos de 2 puntos
    }

    double total = 0.0;
    NodoLista* actual = head;
    while (actual->getSiguiente() != nullptr) {   // parejas (Pi, Pi+1)
        PuntoGPS a = actual->getPuntoGPS();
        PuntoGPS b = actual->getSiguiente()->getPuntoGPS();
        total += a.distancia3D(b);
        actual = actual->getSiguiente();
    }
    return total;
}

double ListaPuntosGPS::velocidadMedia() const {
    if (head == nullptr || head->getSiguiente() == nullptr) {
        return 0.0;   // menos de 2 puntos
    }

    // Marca temporal del último nodo
    NodoLista* ultimo = head;
    while (ultimo->getSiguiente() != nullptr) {
        ultimo = ultimo->getSiguiente();
    }

    unsigned long tPrimero = head->getPuntoGPS().getTimestamp();
    unsigned long tUltimo = ultimo->getPuntoGPS().getTimestamp();
    if (tUltimo <= tPrimero) {
        return 0.0;   // diferencia de tiempo nula
    }

    return distanciaTotal() / static_cast<double>(tUltimo - tPrimero);
}

double ListaPuntosGPS::velocidadMaxima() const {
    if (head == nullptr || head->getSiguiente() == nullptr) {
        return 0.0;   // menos de 2 puntos
    }

    // El máximo parte de la primera velocidad calculada
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

// ===============================================================
// ListaPuntosGPS: representación (Sesión 2)
// ===============================================================

string ListaPuntosGPS::aCadena() const {
    string resultado;
    NodoLista* actual = head;
    while (actual != nullptr) {
        if (actual != head) {
            resultado += "\n";   // separador ANTES, salvo el primero
        }
        resultado += actual->getPuntoGPS().aCadena();
        actual = actual->getSiguiente();
    }
    return resultado;
}
