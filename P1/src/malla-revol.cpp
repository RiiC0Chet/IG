// Nombre: Jose Luis Apellidos: Rico Ramos Titulación: GIADE
// email: jlricoramos@correo.ugr.es DNI o pasaporte: 20080849D

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
   // COMPLETAR: Práctica 4: crear tablas de normales 
   vector<Tupla3f> nor_arist_per;           // vector de normales de las aristas del perfil
   vector<Tupla3f> nor_vert_per;            // vector de normales de los vertices del perfil
   Tupla3f nor_ari;                         // valor auxiliar para ir calculando nor_arist_per
   Tupla3f nor_vert;                        // valor auxiliar para ir calculando nor_vert_per

   // Calculamos las normales de las aristas
   for(uint i = 1; i < perfil.size(); i++){
      nor_ari = MAT_Rotacion(-90,{0.0,0.0,1.0})*(perfil[i] - perfil[i-1]);
      if(nor_ari.lengthSq() != 0.0)
         nor_ari = nor_ari.normalized();
      nor_arist_per.push_back(nor_ari);
   }

   // Calculamos las normales de los vertices
   nor_vert_per.push_back(nor_arist_per[0]);
   for(uint i = 0; i < nor_arist_per.size() - 1; i++){
      nor_vert = (nor_arist_per[i] + nor_arist_per[i+1]).normalized();
      nor_vert_per.push_back(nor_vert);
   }
   nor_vert_per.push_back(nor_arist_per[nor_arist_per.size()-1]);

   // y calcular coordenadas de textura
   vector<float> d;
   vector<float> t;

   // Calculo de d
   for(uint i = 0; i < perfil.size() - 1; i++){
      d.push_back(sqrt((perfil[i+1] - perfil[i]).lengthSq()));
   }

   // Calculamos el normalizador
   float sum = 0.0f;
   for(uint i = 0; i < d.size(); i++){
      sum += d[i];
   }

   // Inicializamos t para simplificar el algoritmo
   for(uint i = 0; i < perfil.size(); i++){
      t.push_back(0.0);
   }

   // Calculo de t
   for(uint i = 0; i < perfil.size(); i++){
      for(uint j = 0; j < i; j++){
         t[i] += d[j];
      }
      t[i] = t[i]/sum*1.0f;
   }

   // COMPLETAR: Práctica 2: completar: creación de la malla....
   vertices.clear();
   for(uint i = 0; i < num_copias; i++){
      for (uint j = 0; j < perfil.size(); j++){
         // Creamos los vertices
         Tupla3f aux = MAT_Rotacion(360.0f*i/(num_copias-1)*1.0,{0,1,0})*perfil.at(j);
         vertices.push_back(aux);
         // Creamos la tabla de normales de vertices
         aux = MAT_Rotacion(360.0f*i/(num_copias-1)*1.0,{0,1,0})*nor_vert_per[j];
         nor_ver.push_back(aux);
         // Añadimos las coordenadas de textura
         cc_tt_ver.push_back({i/(num_copias-1.0f),1-t[j]});
         CError();
      }
   }
   CError();

   // Creamos las caras
   for(uint i = 0; i < num_copias - 1; i++){
      for (uint j = 0; j < perfil.size() - 1; j++){
         uint k = i*perfil.size() + j;
         triangulos.push_back({k, k + (uint)perfil.size(),k + (uint)perfil.size() + 1});
         triangulos.push_back({k, k + (uint)perfil.size() + 1, k + 1});
      }
   }
   CError();
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
      x = 1 * cos(angulo*M_PI/180);
      y = 1 * sin(angulo*M_PI/180);

      perfil.push_back({x,y,0});
   }

   inicializar(perfil,nperfiles);

}


// -----------------------------------------------------------------------------
// Constructor cilindro

Cilindro::Cilindro(const int num_verts_per, const unsigned nperfiles) 
{
   
   // Creamos el perfil de la esfera con el numero de vertices
   float y,
         decremento = 1.0/(num_verts_per-1),
         aux = 1.0;
   // Creamos el vector de tuplas donde se va a almacenar la instancia 0 del perfil
   std::vector<Tupla3f>  perfil;

   // Introducimos tapa abajo
   perfil.push_back({0,0,0});
   

   for(int i = 0; i < num_verts_per; i++){
      perfil.push_back({1,float(i*1.0/(num_verts_per - 1)*1.0),0});
   }


   // Introducimos tapa arriba
   perfil.push_back({0,1.0,0});
   

   inicializar(perfil,nperfiles);
}

// -----------------------------------------------------------------------------
// Constructor cono

Cono::Cono(const int num_verts_per, const unsigned nperfiles) 
{
   // Creamos el perfil de la esfera con el numero de vertices
   float y,
         decremento = 1.0/(num_verts_per-1),
         x_aux = 1.0;
   // Creamos el vector de tuplas donde se va a almacenar la instancia 0 del perfil
   std::vector<Tupla3f>  perfil;

   // Introducimos el punto de arriba
   perfil.push_back({0,1.0,0});

   for(int i = 0;i<num_verts_per-2;i++)
   {
      y= -x_aux + 1;
      perfil.push_back({x_aux,y,0});
      x_aux-=decremento; 
   }

   // Introducimos el punto de abajo
   perfil.push_back({1.0,0,0});

   //Introducimos tapa abajo
   perfil.push_back({0,0,0});
   
   inicializar(perfil,nperfiles);

}

// ----------------------------------Examen juanan-------------------------------------------
Aro::Aro( const int num_verts_per,
                  const unsigned nperfiles, float rInterno, float rEje, Tupla3f color)
{
   std::vector<Tupla3f> perfil;
    float radianes_rotacion = 2.0*M_PI / num_verts_per;
    //float desfase = -M_PI/2;
   for(int i=0; i<=num_verts_per; i++)
      perfil.push_back({rEje + rInterno*cos(i*radianes_rotacion) ,rInterno*sin(i*radianes_rotacion),0.0});

   inicializar (perfil, nperfiles);

   for(int i=0; i < vertices. size(); i++)
      col_ver.push_back(color);
}

HiperPeon::HiperPeon(const int num_verts_per,const unsigned nperfiles)
{
   std::vector<Tupla3f> perfil;
   float radianes_rotacion = M_PI / (num_verts_per-1);
   float desfase = M_PI/2;
   for(int i=0; i<num_verts_per; i++)
   {
      Tupla3f t = {cos(desfase - i*radianes_rotacion ) ,sin(desfase-i*radianes_rotacion) + 2, 0.0};
      perfil.push_back(MAT_Escalado(2.0,1.0,1.0)*t); //Para intentar ponerlos de arriba a abajo
   }

   for(int i=0; i<num_verts_per; i++)
   {
      perfil.push_back({1.0 - (i*(1.0/(num_verts_per-1))),(i*(1.0/(num_verts_per-1))), 0.0});
   }

   inicializar(perfil, nperfiles);

   }

   // -----------------------------------------------------------------------------
   void MallaBarrido::inicializar(
      const std::vector<Tupla3f> & perfil,     // tabla de vértices del perfil original
      const unsigned               num_copias  // número de copias del perfil
   ) {
      float altura_sumar = 1.0/(num_copias-1);
      for(int i=0; i < num_copias; i++){
         for(int j=0; j<perfil.size(); j++){
            Tupla3f q= perfil.at(j);
            q(Y)=i*altura_sumar;
            vertices.push_back(q);
         }
      }
      int m = perfil.size();
      for(int i=0; i < num_copias-1; i++){
         for(int j = 0; j < m-1;j++ ){
            int k = i*m + j;
            triangulos.push_back({k,k+m,k+m+1});
            triangulos.push_back({k,k+m+1, k+1});
         }
      }
   }

   CilindroBarrido::CilindroBarrido(const int num_verts_per, const unsigned nperfiles){
      std::vector<Tupla3f> perfil;
      float radianes_rotacion = M_PI / (num_verts_per-1);
      for(int i=0; i<num_verts_per; i++){
         perfil.push_back({cos(i*radianes_rotacion ) ,0.0, sin(i*radianes_rotacion)}); //Para intentar ponerlos de arriba a abajo
      }

      inicializar(perfil, nperfiles);

   }
   // -----------------------------------------------------------------------------