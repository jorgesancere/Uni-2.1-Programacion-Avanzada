#ifndef LISTAPUNTOSGPS_H
#define LISTAPUNTOSGPS_H

#include <string>
#include "PuntoGPS.h"

using namespace std;

class NodoLista {
public:
    // Constructores, asignacion y destructor
    NodoLista();
    NodoLista(NodoLista*, const PuntoGPS&);
    NodoLista(const NodoLista&);
    NodoLista& operator=(const NodoLista&);
    ~NodoLista();

    // Getters y setters
    NodoLista* getSiguiente() const;
    PuntoGPS getPuntoGPS() const;
    void setSiguiente(NodoLista*);
    void setPuntoGPS(const PuntoGPS&);

private:
    NodoLista* siguiente;
    PuntoGPS puntoGPS;
};

class IteradorLista {
public:
    // Constructores, asignacion y destructor
    IteradorLista();
    IteradorLista(const IteradorLista&);
    ~IteradorLista();
    IteradorLista& operator=(const IteradorLista&);

    // Avanzar y comparar
    void step();
    bool operator==(const IteradorLista&) const;
    bool operator!=(const IteradorLista&) const;

private:
    NodoLista* pt;

    friend class ListaPuntosGPS;
};

class ListaPuntosGPS {
public:
    // Constructores, asignacion y destructor
    ListaPuntosGPS();
    ListaPuntosGPS(const ListaPuntosGPS&);
    ListaPuntosGPS& operator=(const ListaPuntosGPS&);
    ~ListaPuntosGPS();

    // Iteradores
    IteradorLista begin() const;
    IteradorLista end() const;
    PuntoGPS getPuntoGPS(IteradorLista) const;

    // Consultas
    bool esVacia() const;
    int longitud() const;

    // Busqueda
    bool existeTimestamp(unsigned long) const;
    IteradorLista buscarPorTimestamp(unsigned long) const;
    int contarMismaUbicacion(const PuntoGPS&) const;

    // Insertar y asignar
    bool insertar(const PuntoGPS&);

    bool asignar(IteradorLista, const PuntoGPS&);

    // Borrar
    bool borrarPrimero();
    bool borrarUltimo();
    bool borrar(IteradorLista&);
    bool borrarPorTimestamp(unsigned long);

    // Filtros
    ListaPuntosGPS filtrarRangoTimestamp(unsigned long, unsigned long) const;
    ListaPuntosGPS filtrarDispositivo(const string&) const;

    // Trayectoria
    double distanciaTotal() const;
    double velocidadMedia() const;
    double velocidadMaxima() const;

    // Texto
    string aCadena() const;

private:
    NodoLista* head;

    // Auxiliares privados
    void vaciar();
    void copiarDesde(const ListaPuntosGPS&);
    void agregarAlFinal(NodoLista*&, const PuntoGPS&);
};

#endif
