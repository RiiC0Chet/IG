// Nombre: Jose Luis Apellidos: Rico Ramos Titulación: GIADE
// email: jlricoramos@correo.ugr.es DNI o pasaporte: 20080849D
#include "ig-aux.h"
#include "escena.h"

#include "objeto3d.h"
#include "malla-ind.h"
#include "malla-revol.h"
#include "practicas.h"
#include "camara.h"
#include "materiales-luces.h"
#include "seleccion.h"
#include "modelo-jer.h"


#include "Examen_1.h"

//----------------Clase VaritaInicial--------------------

VaritaInicial::VaritaInicial()
{
    agregar(MAT_Traslacion({2.0,-1.0,0.0}));
    agregar(new Cubo());
}   

//----------------Clase VaritaFianl--------------------

VaritaFinal::VaritaFinal()
{
    int indice_rot = agregar(MAT_Rotacion(10,{0.0,0.0,1.0}));
    agregar(MAT_Escalado(1.0,10.0,1.0));
    agregar(new VaritaInicial());

    pm_rot = leerPtrMatriz(indice_rot);
}

unsigned VaritaFinal::leerNumParametros() const
{
    return 1;
}

void VaritaFinal::actualizarEstadoParametro( const unsigned iParam, const float t_sec )
{
    assert(iParam < leerNumParametros() && iParam >=0);

    switch(iParam)
    {
        case 0:
            // oscilamos entre 10 (360+10) y 90 (360+90) grados respecto al eje Y, 2 veces por segundo
            float v = ((370+450)/2)+((450-370)/2)*sin(2*M_PI*t_sec*2);
            *pm_rot = MAT_Rotacion( v , { 0.0, 0.0, 1.0 } ) ;
        break;
    }
}
//----------------Clase SoporteInicial--------------------

SoporteInicial::SoporteInicial()
{
    agregar(MAT_Escalado(1.0,15.0,1.0));
    agregar(new Cilindro(5,20));
}

//----------------Clase SoporteFinal--------------------


SoporteFinal::SoporteFinal(int M)
{
   const int GRADOS_CRIC = 360;
   assert(M>=2);

    // Agregamos el soporte inicial
    agregar (new SoporteInicial());

   float grados_int_inicial = GRADOS_CRIC/M;
                                       // mientras que el angulo interno sea distinto al ultimo a generar  ( sum_grados_interno < (GRADOS_CRIC - (GRADOS_CRIC/n)/2) ) && 
   
   // Introducimos el desplazamiento para que las baras se encuentren al borde del cilindro
   agregar(MAT_Traslacion({0.0,15.0,0.0}));

   for(int i = 0 ;i < M;i++)
   {                       // Puntas de la estrella
      agregar(MAT_Rotacion(grados_int_inicial,{0.0,1.0,0.0}));
      lista_varitas.push_back(new VaritaFinal());
      agregar(lista_varitas.at(i));
   }
}

unsigned SoporteFinal::leerNumParametros() const
{
    return 1;
}

void SoporteFinal::actualizarEstadoParametro( const unsigned iParam, const float t_sec )
{
    assert(iParam < leerNumParametros() && iParam >=0);

    switch(iParam)
    {
        case 0:
            for(int i = 0; i <lista_varitas.size();i++)
                lista_varitas.at(i)->actualizarEstadoParametro(iParam,t_sec);
        break;
    }
}

//------------------------------------------------