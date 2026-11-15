#ifndef LISTAPUNTOSGPS_H
#define LISTAPUNTOSGPS_H

#include <string>
#include "PuntoGPS.h"

using namespace std;

// ---------------------------------------------------------------
// NodoLista
// ---------------------------------------------------------------
class NodoLista {
public:
    NodoLista();
    NodoLista(NodoLista*, const PuntoGPS&);
    NodoLista(const NodoLista&);
    NodoLista& operator=(const NodoLista&);
    ~NodoLista();

    NodoLista* getSiguiente() const;
    PuntoGPS getPuntoGPS() const;
    void setSiguiente(NodoLista*);
    void setPuntoGPS(const PuntoGPS&);

private:
    NodoLista* siguiente;
    PuntoGPS puntoGPS;
};

// ---------------------------------------------------------------
// IteradorLista
// ---------------------------------------------------------------
class IteradorLista {
public:
    IteradorLista();
    IteradorLista(const IteradorLista&);
    ~IteradorLista();
    IteradorLista& operator=(const IteradorLista&);

    void step();
    bool operator==(const IteradorLista&) const;
    bool operator!=(const IteradorLista&) const;

private:
    NodoLista* pt;

    friend class ListaPuntosGPS;
};

// ---------------------------------------------------------------
// ListaPuntosGPS (Sesiones 1 a 4)
// ---------------------------------------------------------------
class ListaPuntosGPS {
public:
    // Forma canónica
    ListaPuntosGPS();
    ListaPuntosGPS(const ListaPuntosGPS&);
    ListaPuntosGPS& operator=(const ListaPuntosGPS&);
    ~ListaPuntosGPS();

    // Iteración
    IteradorLista begin() const;
    IteradorLista end() const;
    PuntoGPS getPuntoGPS(IteradorLista) const;

    // Consulta básica
    bool esVacia() const;
    int longitud() const;

    // Búsqueda y consulta
    bool existeTimestamp(unsigned long) const;
    IteradorLista buscarPorTimestamp(unsigned long) const;
    int contarMismaUbicacion(const PuntoGPS&) const;

    // Inserción ordenada
    bool insertar(const PuntoGPS&);

    // Modificación de un elemento
    bool asignar(IteradorLista, const PuntoGPS&);

    // Borrado
    bool borrarPrimero();
    bool borrarUltimo();
    bool borrar(IteradorLista&);
    bool borrarPorTimestamp(unsigned long);

    // Filtrado (devuelven una lista nueva)
    ListaPuntosGPS filtrarRangoTimestamp(unsigned long, unsigned long) const;
    ListaPuntosGPS filtrarDispositivo(const string&) const;

    // Cálculos sobre la trayectoria completa
    double distanciaTotal() const;
    double velocidadMedia() const;
    double velocidadMaxima() const;

    // Representación
    string aCadena() const;

private:
    NodoLista* head;

    // Métodos auxiliares (privados, permitidos)
    void vaciar();
    void copiarDesde(const ListaPuntosGPS&);
    void agregarAlFinal(NodoLista*&, const PuntoGPS&);
};

#endif
