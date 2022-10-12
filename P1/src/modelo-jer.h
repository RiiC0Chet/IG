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
};


#endif