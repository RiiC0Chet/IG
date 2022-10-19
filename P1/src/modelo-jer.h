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
class PlataformaInferior : public NodoGrafoEscena
{
    public:
    PlataformaInferior();
};

//Pilar de la grua (Plataforma inferior+Cilindro)
class PilarGrua : public NodoGrafoEscena
{
    public:
    PilarGrua();
};
#endif