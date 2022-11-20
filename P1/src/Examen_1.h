// Nombre: Jose Luis Apellidos: Rico Ramos Titulación: GIADE
// email: jlricoramos@correo.ugr.es DNI o pasaporte: 20080849D

#ifndef IG_EXAMEN_1_HPP
#define IG_EXAMEN_1_HPP

#include "grafo-escena.h"
#include "malla-revol.h"
#include <vector>

class VaritaInicial : public NodoGrafoEscena
{   
    public:
    VaritaInicial();
};

class VaritaFinal : public NodoGrafoEscena
{
    protected:
    Matriz4f * pm_rot = nullptr ;
    
    public:
    VaritaFinal();

    virtual unsigned leerNumParametros() const;

    virtual void actualizarEstadoParametro( const unsigned iParam, const float t_sec );
};

class SoporteInicial : public NodoGrafoEscena
{   
    public:
    SoporteInicial();
};

class SoporteFinal : public NodoGrafoEscena
{   
    protected:
    std::vector<VaritaFinal *> lista_varitas;

    public:
    SoporteFinal(int M);

    virtual unsigned leerNumParametros() const;

    virtual void actualizarEstadoParametro( const unsigned iParam, const float t_sec );
};

#endif