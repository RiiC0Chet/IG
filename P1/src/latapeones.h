#ifndef LATAPEONES_HPP
#define LATAPEONES_HPP

#include "grafo-escena.h"

class Lata : public NodoGrafoEscena
{
    public: 
    Lata();
};

class PeonMadera : public NodoGrafoEscena
{
    public: 
    PeonMadera();
};

class PeonBlanco : public NodoGrafoEscena
{
    public: 
    PeonBlanco();
};

class PeonNegro : public NodoGrafoEscena
{
    public: 
    PeonNegro();
};

class LataPeones : public NodoGrafoEscena
{
    public: 
    LataPeones();
};


#endif // GRAFO_ESCENA_HPP