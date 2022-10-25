#ifndef IG_MODELOJER_HPP
#define IG_MODELOJER_HPP

#include "grafo-escena.h"
#include "malla-revol.h"

class C : public NodoGrafoEscena
{
    protected:

    Matriz4f * pm_tras = nullptr ;
    public:
    C(float traslacion);

    virtual unsigned leerNumParametros() const;

    virtual void actualizarEstadoParametro( const unsigned iParam, const float t_sec );
};

// Rectangulo de la parte inferior de la grua
class PlataformaCuadrada : public NodoGrafoEscena
{
    public:
    PlataformaCuadrada();
};

//Pilar de la grua (Plataforma inferior+Cilindro)
class PilarGrua : public NodoGrafoEscena
{
    public:
    PilarGrua();
};

// Plataforma desplazante

class PlataformaDesplazante : public NodoGrafoEscena
{
    protected:

    Matriz4f * pm_tras = nullptr ;
    public:
    PlataformaDesplazante();

    virtual unsigned leerNumParametros() const;

    virtual void actualizarEstadoParametro( const unsigned iParam, const float t_sec );
};

// Plataforma arriba

class PlataformaArriba : public NodoGrafoEscena
{
    protected:
    Matriz4f * pm_tras = nullptr ;
    PlataformaDesplazante * plataforma_desplazante = nullptr;
    public:
    PlataformaArriba();

    virtual unsigned leerNumParametros() const;

    virtual void actualizarEstadoParametro( const unsigned iParam, const float t_sec );
};

// Grua completa

class Grua : public NodoGrafoEscena
{
    protected:
    Matriz4f * pm_rot = nullptr ;
    PlataformaArriba * plataforma_arriba = nullptr;

    public:
    Grua(float alpha);
    virtual unsigned leerNumParametros() const;

    virtual void actualizarEstadoParametro( const unsigned iParam, const float t_sec );

};
#endif