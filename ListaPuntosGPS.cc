#include "ListaPuntosGPS.h"
#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>

#include "ListaPuntosGPS.h"

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
// ListaPuntosGPS
// ===============================================================

ListaPuntosGPS::ListaPuntosGPS() : head(nullptr) {
}

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

// Versión rudimentaria (Sesión 1): inserta siempre al final
bool ListaPuntosGPS::insertar(const PuntoGPS& p) {
    NodoLista* nuevo = new NodoLista(nullptr, p);

    if (head == nullptr) {
        head = nuevo;
    } else {
        NodoLista* actual = head;
        while (actual->getSiguiente() != nullptr) {
            actual = actual->getSiguiente();
        }
        actual->setSiguiente(nuevo);
    }
    return true;
}