// Nombre: Jose Luis Apellidos: Rico Ramos Titulación: GIADE
// email: jlricoramos@correo.ugr.es DNI o pasaporte: 20080849D

// *********************************************************************
// **
// ** Gestión de una grafo de escena (declaraciones)
// ** Copyright (C) 2014-2017 Carlos Ureña
// **
// ** This program is free software: you can redistribute it and/or modify
// ** it under the terms of the GNU General Public License as published by
// ** the Free Software Foundation, either version 3 of the License, or
// ** (at your option) any later version.
// **
// ** This program is distributed in the hope that it will be useful,
// ** but WITHOUT ANY WARRANTY; without even the implied warranty of
// ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// ** GNU General Public License for more details.
// **
// ** You should have received a copy of the GNU General Public License
// ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
// **
// *********************************************************************

#ifndef GRAFO_ESCENA_HPP
#define GRAFO_ESCENA_HPP

#include "tup_mat.h"
#include "objeto3d.h"
#include "materiales-luces.h"

using namespace tup_mat ;

// *********************************************************************
// declaración adelantada de estructura para un nodo del grafo de escena

class NodoGrafoEscena ;

// *********************************************************************
// tipo enumerado con los tipos de entradas del nodo del grafo de escena

enum class TipoEntNGE { objeto, transformacion, material, noInicializado } ;

// *********************************************************************
// Entrada del nodo del Grafo de Escena

struct EntradaNGE
{
   TipoEntNGE tipo ;   // objeto, transformacion, material
   union
   {  Objeto3D * objeto ;  // ptr. a un objeto (no propietario)
      Matriz4f * matriz ;  // ptr. a matriz 4x4 transf. (propietario)
      Material * material ; // ptr. a material (no propietario)
   } ;
   // constructores (uno por tipo)
   EntradaNGE( Objeto3D * pObjeto ) ;      // (copia solo puntero)
   EntradaNGE( const Matriz4f & pMatriz ); // (crea copia en el heap)
   EntradaNGE( Material * pMaterial ) ;    // (copia solo puntero)
   ~EntradaNGE() ;
} ;

// *********************************************************************
// Nodo del grafo de escena: es un objeto 3D parametrizado, que contiene una lista de entradas

class NodoGrafoEscena : public Objeto3D
{
   protected:
   // COMPLETAR: práctica 3: definir variables y métodos privados del nodo
   // .......
   std::vector<EntradaNGE> entradas ; // vector de entradas
   bool centro_calculado = false;
   public:

   NodoGrafoEscena() ;

   // visualiza usando OpenGL
   virtual void visualizarGL( ContextoVis & cv ) ;

   // visualizar pura y simplemente la geometría, sin colores, normales, coord. text. etc...
   // (se supone que el estado de OpenGL está fijado antes de esta llamada de alguna forma adecuada)
   virtual void visualizarGeomGL( ContextoVis & cv ) ;

   // añadir una entrada al final, hace copia de la entrada
   // devuelve indice de la entrada dentro del vector de entradas
   unsigned agregar( const EntradaNGE & entrada );

   // construir una entrada y añadirla (al final)
   unsigned agregar( Objeto3D * pObjeto ); // objeto (copia solo puntero)
   unsigned agregar( const Matriz4f & pMatriz ); // matriz (copia objeto)
   unsigned agregar( Material * pMaterial ); // material (copia solo puntero)

   // devuelve el puntero a la matriz en la i-ésima entrada
   Matriz4f * leerPtrMatriz( unsigned iEnt );

   // método para buscar un objeto con un identificador
   virtual bool buscarObjeto( const int ident_busc, const Matriz4f & mmodelado,
                    Objeto3D ** objeto, Tupla3f & centro_wc )  ;

   // si 'centro_calculado' es 'false', recalcula el centro usando los centros
   // de los hijos (el punto medio de la caja englobante de los centros de hijos)
   virtual void calcularCentroOC() ;


} ;
// *********************************************************************

class GrafoEstrellaX : public NodoGrafoEscena
{
   protected:

   Matriz4f * pm_rot = nullptr ;
   public:
   GrafoEstrellaX(int n, float alpha);

   virtual unsigned leerNumParametros() const;

   virtual void actualizarEstadoParametro( const unsigned iParam, const float t_sec );
};

// *********************************************************************

class ConoX : public NodoGrafoEscena
{
   public:
   ConoX(float x, float y, float grados);
};

// *********************************************************************

class GrafoCubos : public NodoGrafoEscena
{
   protected:

   Matriz4f * pm_rot = nullptr ;
   int cara_;

   public:
   GrafoCubos(Tupla3f cara);

   virtual unsigned leerNumParametros() const;

   virtual void actualizarEstadoParametro( const unsigned iParam, const float t_sec );

};

class GrafoCuboX : public NodoGrafoEscena
{
   protected:
   GrafoCubos *cubo_1, *cubo_2, *cubo_3, *cubo_4,
               *cubo_5, *cubo_6;
   public:
   GrafoCuboX();

   virtual unsigned leerNumParametros() const;

   virtual void actualizarEstadoParametro( const unsigned iParam, const float t_sec );

};

class NodoCubo24 : public NodoGrafoEscena
{
   public:
   NodoCubo24();

};
// *************************** Ejercicio adicional P4******************************************

class NodoDiscoP4 : public NodoGrafoEscena
{
   public:
   NodoDiscoP4();

};

// *************************** Ejercicio adicional P5******************************************

class GrafoEsferasP5 : public NodoGrafoEscena
{
   public:
   GrafoEsferasP5();

};

class GrafoEsferasP5_2 : public NodoGrafoEscena
{
   public:
   GrafoEsferasP5_2();

};
// ******************************* Examen P4,5**************************************

class EsferaEXP5 : public NodoGrafoEscena
{
   public:
   EsferaEXP5(int n);
   bool seleccionada = false;
};

class AnilloEXP5 : public NodoGrafoEscena
{
   public:
   AnilloEXP5( int n);

};

class NodoEXP4 : public NodoGrafoEscena
{
   public:
   NodoEXP4();

};

#endif // GRAFO_ESCENA_HPP

// *********************************************************************
