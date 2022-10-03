// *********************************************************************
// **
// ** Informática Gráfica, curso 2019-20
// ** Implementación de la clase 'MallaRevol'
// **
// *********************************************************************

#include "ig-aux.h"
#include "lector-ply.h"
#include "malla-revol.h"

using namespace std ;

// *****************************************************************************




// Método que crea las tablas de vértices, triángulos, normales y cc.de.tt.
// a partir de un perfil y el número de copias que queremos de dicho perfil.
void MallaRevol::inicializar
(
   const std::vector<Tupla3f> & perfil,     // tabla de vértices del perfil original
   const unsigned               num_copias  // número de copias del perfil
)
{
   // COMPLETAR: Práctica 2: completar: creación de la malla....

   int k; // auxiliar para la creacion de los nuevos triangulos
   float x_aux,z_aux; // auxiliares para la creacion de las nuevas tuplas

   // Creamos la tabla de vertices
   for(int i=0;i<=(num_copias-1);i++)
   {
      for(int j=0;j<=(perfil.size()-1);j++)
      {
         // calculamos las nuevas tuplas de revolucion
         x_aux = perfil.at(j)(0) * cos( (((i*2*3.141516) / (num_copias-1)))  );
         z_aux = perfil.at(j)(0) * sin( (((i*2*3.141516) / (num_copias-1)))  );

         // introducimos el nuevo punto en la tabla de vertices
         vertices.push_back({x_aux,perfil.at(j)(1),z_aux});
      }
   }

   // Creamos la tabla de triangulos
   for(int i=0;i<=(num_copias-2);i++)
   {
      for(int j=0;j<=(perfil.size()-2);j++)
      {
         // asignamos la variable k
         k = (i*perfil.size()) + j;

         // Creamos lso 2 nuevos triangulos asociados con cada uno de los nuevos vertices
         triangulos.push_back({k,k+perfil.size(),k+perfil.size()+1});
         triangulos.push_back({k,k+perfil.size()+1,k+1});
      }
   }

}

// -----------------------------------------------------------------------------
// constructor, a partir de un archivo PLY

MallaRevolPLY::MallaRevolPLY
(
   const std::string & nombre_arch,
   const unsigned      nperfiles
)
{
   ponerNombre( std::string("malla por revolución del perfil en '"+ nombre_arch + "'" ));
   // COMPLETAR: práctica 2: crear la malla de revolución
   // Leer los vértice del perfil desde un PLY, después llamar a 'inicializar'
   // ...........................
   
   // Creamos el vector de tuplas donde se va a almacenar la instancia 0 del perfil
   std::vector<Tupla3f>  perfil;

   // Lewmoa los vertices del archivo y lo almacenamos en el vector de tuplas perfil
   LeerVerticesPLY(nombre_arch,perfil);

   // Inicializamos las mallas de vectores y triangulos
   inicializar(perfil,nperfiles);


}

// -----------------------------------------------------------------------------
// Constructor esfera
Esfera::Esfera( const int num_verts_per, const unsigned nperfiles) 
{
   // Creamos el perfil de la esfera con el numero de vertices
   float x,y;
   // Creamos el vector de tuplas donde se va a almacenar la instancia 0 del perfil
   std::vector<Tupla3f>  perfil;

   for(int  angulo = -90; angulo<=90 ;angulo+= 180/num_verts_per )
   {
      // el radio es la unidad
      x = 1 * cos(angulo*3.1441516/180);
      y = 1 * sin(angulo*3.1441516/180);

      perfil.push_back({x,y,0});
   }

   inicializar(perfil,nperfiles);

}

