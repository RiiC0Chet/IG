// Nombre: Jose Luis Apellidos: Rico Ramos Titulación: GIADE
// email: jlricoramos@correo.ugr.es DNI o pasaporte: 20080849D

// *********************************************************************
// **
// ** Informática Gráfica, curso 2020-21
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

#include "ig-aux.h"
#include "malla-ind.h"   // declaración de 'ContextoVis'
#include "lector-ply.h"

// *****************************************************************************
// funciones auxiliares

// *****************************************************************************
// métodos de la clase MallaInd.

MallaInd::MallaInd()
{
   // nombre por defecto
   ponerNombre("malla indexada, anónima");
}
// -----------------------------------------------------------------------------

MallaInd::MallaInd( const std::string & nombreIni )
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre(nombreIni) ;
}

//-----------------------------------------------------------------------------
// calcula la tabla de normales de triángulos una sola vez, si no estaba calculada

void MallaInd::calcularNormalesTriangulos()
{

   // si ya está creada la tabla de normales de triángulos, no es necesario volver a crearla
   const unsigned nt = triangulos.size() ;
   assert( 1 <= nt );
   if ( 0 < nor_tri.size() )
   {
      assert( nt == nor_tri.size() );
      return ;
   }

   // COMPLETAR: Práctica 4: creación de la tabla de normales de triángulos
   // ....
   
   Tupla3f v_1,v_2,v_3;
   for(int i=0;i<triangulos.size();i++)
   {
      v_1 = vertices[triangulos[i](1)] - vertices[triangulos[i](0)];
      v_2 = vertices[triangulos[i](2)] - vertices[triangulos[i](0)];

      v_3 = v_1.cross(v_2);

      nor_tri.push_back(v_3.normalized());

   }

}


// -----------------------------------------------------------------------------
// calcula las dos tablas de normales

void MallaInd::calcularNormales()
{
   // COMPLETAR: en la práctica 4: calculo de las normales de la malla
   // se debe invocar en primer lugar 'calcularNormalesTriangulos'
   // .......
   
   calcularNormalesTriangulos();
   
   std::vector<Tupla3f> vertex_normals(vertices.size(), Tupla3f(0, 0, 0));
   for (int i = 0; i < triangulos.size(); i++) 
   {
      Tupla3u t = triangulos[i];
      vertex_normals[t[0]] = vertex_normals[t[0]] + nor_tri[i];
      vertex_normals[t[1]] = vertex_normals[t[1]] + nor_tri[i];
      vertex_normals[t[2]] = vertex_normals[t[2]] + nor_tri[i];
   }
   
   for (int i = 0; i < vertex_normals.size(); i++) 
   {
      if(vertex_normals[i].lengthSq() > 0.0)
         nor_ver.push_back(vertex_normals[i].normalized());
      else
         nor_ver.push_back(vertex_normals[i]);
   }

}



// --------------------------------------------------------------------------------------------

void MallaInd::visualizarGL( ContextoVis & cv )
{
   using namespace std ;
   assert( cv.cauce != nullptr );
   CError();

   if ( cv.visualizando_normales )
   {  
      visualizarNormales( cv );
      return ;
   }

   if ( triangulos.size() == 0 || vertices.size() == 0 )
   {  cout << "advertencia: intentando dibujar malla vacía '" << leerNombre() << "'" << endl << flush ;
      return ;
   }

   // guardar el color previamente fijado y fijar el color del objeto actual
   const Tupla4f color_previo = leerFijarColVertsCauce( cv );

   // COMPLETAR: práctica 1: si el nombre del VAO es 0, crear el VAO con sus VBOs:
   //   * en primer lugar se crea y activa el VAO , con 'CrearVAO'.
   //   * después se añade el VBO con la tabla de coordenadas de posición, 'CrearVBOAtrib'.
   //   * se añade el VBO con la tabla de índices (la tabla de triángulos), con 'CrearVBOInd'.
   //   * finalmente se añaden al VAO los VBOs con tablas de atributos (opcionaes) que haya, con 'CrearVBOAtrib'.
   // si el VAO ya está creado, (nombre_vao > 0), activarlo, con 'glBindVertexArray'
   //
   //  hay que tener en cuenta que el nombre del VAO y los nombres de losVBOs deben quedar en las correspondientes 
   //  variables de instancia. Estos nombres son los devueltos por las 
   //  funciones 'CrearVAO', 'CrearVBOAtrib' y 'CrearVBOInd'.
   //

   if(nombre_vao == 0) // si el VAO no esta creado
   {
      // creamos y activamos el VAO
      nombre_vao = CrearVAO();
      
      // después se añade el VBO con la tabla de coordenadas de posición
      if(vertices.size() > 0 ) nombre_vbo_pos = CrearVBOAtrib(ind_atrib_posiciones,vertices);

      //se añade el VBO con la tabla de índices (la tabla de triángulos)
      if (triangulos.size() >0) nombre_vbo_tri = CrearVBOInd( triangulos );

      // finalmente se añaden al VAO los VBOs con tablas de atributos (opcionaes) que haya
      if (col_ver.size() >0) nombre_vbo_col = CrearVBOAtrib( ind_atrib_colores, col_ver );
      if (nor_ver.size() >0) nombre_vbo_nor = CrearVBOAtrib( ind_atrib_normales, nor_ver );
         //if (nor_tri.size() >0) CrearVBOAtrib( ind_atrib_normales, nor_tri );
      if (cc_tt_ver.size()>0) nombre_vbo_cct = CrearVBOAtrib( ind_atrib_coord_text, cc_tt_ver);
   }
   else // si el VAO ya está creado
      glBindVertexArray( nombre_vao ); // activar el VAO

   std::cout<<"gl triangulo size :"<<triangulos.size()<<std::endl;


   // COMPLETAR: práctica 1: visualizar con 'glDrawElements' y desactivar VAO.
   glDrawElements(GL_TRIANGLES,triangulos.size()*3,GL_UNSIGNED_INT,0);
   glBindVertexArray( 0 ); 

   // restaurar el color previamente fijado
   cv.cauce->fijarColor( color_previo );
}


// -----------------------------------------------------------------------------
// visualizar pura y simplemente la geometría, sin colores, normales, coord. text. etc...
// (se supone que el estado de OpenGL está fijado antes de esta llamada de alguna forma adecuada)

void MallaInd::visualizarGeomGL( ContextoVis & cv )
{
   // COMPLETAR: práctica 1: asegurarnos de que el VAO de geometría está creado y activado
   // ....
   if(nombre_vao_geo == 0) // si el VAO no esta creado
   {
      // creamos y activamos el VAO
      nombre_vao_geo = CrearVAO();
      
      // No hace falta ninguno de estos realmente, ya que los VBO estos ya se crearon en el visualGL
      // y como este metodo siempre se llama despues del anterior ya estaran creados

      // Activamos VBO
      glBindBuffer(GL_ARRAY_BUFFER, nombre_vbo_pos);

      // Le decimos donde estan las posiciones
      glVertexAttribPointer(ind_atrib_posiciones, 3,GL_FLOAT , GL_FALSE, 0, 0);

      // Terminamos de activar
      glEnableVertexAttribArray(ind_atrib_posiciones);

      // Activamos el vbo de triangulos
      // No se utiliza GL_ARRAY_BUFFER porque ese es para atributos, no para indices
      glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, nombre_vbo_tri ); 

      CError();
   }
   else // si el VAO ya está creado
      glBindVertexArray( nombre_vao_geo ); // activar el VAO




  // std::cout<<"gl triangulo size :"<<triangulos.size()<<std::endl;
   // COMPLETAR: práctica 1: visualizar con 'glDrawElements' y desactivar VAO.
   glDrawElements(GL_TRIANGLES,triangulos.size()*3,GL_UNSIGNED_INT,0);
   glBindVertexArray( 0 ); 
   CError();
   // COMPLETAR: práctica 1. Visualizar la malla y desactivar VAO
   // ....

}


void MallaInd::visualizarNormales( ContextoVis & cv )
{
   using namespace std ;

   if ( nor_ver.size() == 0 )
   {
      cout << "Advertencia: intentando dibujar normales de una malla que no tiene tabla (" << leerNombre() << ")." << endl ;
      return ;
   }  
   if ( nombre_vao_normales == 0 )
   {  
      for( unsigned i = 0 ; i < vertices.size() ; i++ )
      {  
         segmentos_normales.push_back( vertices[i] );
         segmentos_normales.push_back( vertices[i]+ 0.35f*(nor_ver[i]) );
      }
      nombre_vao_normales = CrearVAO();
      CrearVBOAtrib( ind_atrib_posiciones, segmentos_normales );   
   }
   else 
      glBindVertexArray( nombre_vao_normales );

   cv.cauce->fijarColor( 1.0, 0.5, 0.2 ); // fijar el color (rojo), con el VAO activado.
   glDrawArrays( GL_LINES, 0, segmentos_normales.size() );
   glBindVertexArray( 0 );
   CError();
}

// ****************************************************************************
// Clase 'MallaPLY'

MallaPLY::MallaPLY( const std::string & nombre_arch )
{
   ponerNombre( std::string("malla leída del archivo '") + nombre_arch + "'" );

   // COMPLETAR: práctica 2: leer archivo PLY e inicializar la malla
   // ..........................
   LeerPLY(nombre_arch,vertices,triangulos);

   // COMPLETAR: práctica 4: invocar  a 'calcularNormales' para el cálculo de normales
   // .................
   calcularNormales();
}

// ****************************************************************************
// Clase 'Cubo

Cubo::Cubo()
:  MallaInd( "cubo 8 vértices" )
{

   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7
      } ;



   triangulos =
      {  {0,1,3}, {2,0,3}, // X-
         {3,1,5}, {5,7,3}, // X+ (+4)

         {6,7,5}, {5,4,6}, // Y-
         {4,0,6}, {0,2,6}, // Y+ (+2)

         {2,3,6}, {3,7,6}, // Z-
         {5,1,0}, {5,0,4}  // Z+ (+1)
      } ;
   
   calcularNormales();

}

Cubo24::Cubo24(){
   //ponerIdentificador(5); // práctica 5

  vertices =
      {  { -1.0, -1.0, -1.0 }, // 0.0 y**
         { -1.0, -1.0, -1.0 }, // 0.1 y
         { -1.0, -1.0, -1.0 }, // 0.2 y
         { -1.0, -1.0, +1.0 }, // 1.3 y**
         { -1.0, -1.0, +1.0 }, // 1.4 y--
         { -1.0, -1.0, +1.0 }, // 1.5 y
         { -1.0, +1.0, -1.0 }, // 2.6 y**
         { -1.0, +1.0, -1.0 }, // 2.7 y
         { -1.0, +1.0, -1.0 }, // 2.8 y
         { -1.0, +1.0, +1.0 }, // 3.9 y**
         { -1.0, +1.0, +1.0 }, // 3.10 y--
         { -1.0, +1.0, +1.0 }, // 3.11 y
         { +1.0, -1.0, -1.0 }, // 4.12 y
         { +1.0, -1.0, -1.0 }, // 4.13 y
         { +1.0, -1.0, -1.0 }, // 4.14 y
         { +1.0, -1.0, +1.0 }, // 5.15 y--
         { +1.0, -1.0, +1.0 }, // 5.16 y
         { +1.0, -1.0, +1.0 }, // 5.17 y
         { +1.0, +1.0, -1.0 }, // 6.18 y
         { +1.0, +1.0, -1.0 }, // 6.19 y
         { +1.0, +1.0, -1.0 }, // 6.20 y
         { +1.0, +1.0, +1.0 }, // 7.21 y--
         { +1.0, +1.0, +1.0 }, // 7.22 y
         { +1.0, +1.0, +1.0 }, // 7.23 y
      } ;

   triangulos =
      {  {9,0,3}, {0,9,6}, // X-
         {10,4,15}, {15,21,10}, // X+ (+4)

         {22,16,18}, {16,12,18}, // Y-
         {13,1,19}, {1,7,19}, // Y+ (+2)

         {8,11,20}, {11,23,20}, // Z-
         {17,5,2}, {17,2,14}  // Z+ (+1)
      };

   cc_tt_ver = 
      {  {0.0,1.0},     // 1 y
         {1.0,1.0},     // 4 y
         {1.0,0.0},     // 6 y
         {1.0,1.0},     // 1 y
         {0.0,1.0},     // 2 y
         {1.0,1.0},     // 6 y
         {0.0,0.0},     // 1 y
         {1.0,0.0},     // 4 y
         {1.0,0.0},     // 5 y 
         {1.0,0.0},     // 1 y
         {0.0,0.0},     // 2 y
         {0.0,0.0},     // 5 y
         {1.0,1.0},     // 3 y
         {0.0,1.0},     // 4 y   
         {0.0,0.0},     // 6 y
         {1.0,1.0},     // 2 y 
         {0.0,1.0},     // 3 y
         {0.0,1.0},     // 6 y
         {1.0,0.0},     // 3 y 
         {0.0,0.0},     // 4 y 
         {1.0,1.0},     // 5 y
         {1.0,0.0},     // 2 y 
         {0.0,0.0},     // 3 y 
         {0.0,1.0},     // 5 y  
      };

   calcularNormales();
}

// ****************************************************************************
// Clase 'CuboTejado'

CuboTejado::CuboTejado()
:  MallaInd( "cubo 8 vértices" )
{

   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7
         { +0.0, +2.0, +0.0 }, // 8
      } ;

   for( Tupla3f & v : vertices )
      v = 2.0f*v +Tupla3f({0.0,2.0,0.0});



   triangulos =
      {  //{0,1,3}, {0,3,2}, // X-
         //{4,7,5}, {4,6,7}, // X+ (+4)

         //{0,5,1}, {0,4,5}, // Y-
         //{2,3,7}, {2,7,6}, // Y+ (+2)   // quito cara superior
         //{2,3,8}, {3,7,8}, {7,6,8}, {6,2,8}, // añado tejado 

         //{0,6,4}, {0,2,6}, // Z-
         //{1,5,7}, {1,7,3}  // Z+ (+1)

         {3,1,0}, {3,2,0}, // X-
         {7,5,4}, {7,6,4}, // X+ (+4)

         {5,1,0}, {5,4,0}, // Y-
         //{2,3,7}, {2,7,6}, // Y+ (+2)   // quito cara superior
         {8,3,2}, {8,7,3}, {8,7,6}, {8,6,2}, // añado tejado 

         {6,4,0}, {6,2,0}, // Z-
         {7,5,1}, {7,3,1}  // Z+ (+1)

      } ;
      calcularNormales();
}

// -----------------------------------------------------------------------------------------------

Tetraedro::Tetraedro()
:  MallaInd( "Tetraedro 4 vértices" )
{

   vertices =
      {  { 1.0, 0.0, 0.0 }, // 0
         { 0.0, 1.0, 0.0 }, // 1
         { 0.0, 0.0, 1.0 }, // 2
         { 0.0, 0.0, 0.0 }, // 3
      } ;



   triangulos =
      {  {3,1,0}, {0,1,2},
         {3,0,2}, {3,2,1}
      } ;

   Tupla3f color = {0.25, 0.75, 0.0};
   ponerColor(color);
   calcularNormales();
}

// -----------------------------------------------------------------------------------------------

CuboColores::CuboColores()
:  MallaInd( "cubo 8 vértices" )
{

   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7
      } ;



   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

         {0,5,1}, {0,4,5}, // Y-
         {2,3,7}, {2,7,6}, // Y+ (+2)

         {0,6,4}, {0,2,6}, // Z-
         {1,5,7}, {1,7,3}  // Z+ (+1)
      } ;
   
   col_ver =
      {  { 0.0, 0.0, 0.0 }, // 0
         { 0.0, 0.0, +1.0 }, // 1
         { 0.0, +1.0, 0.0 }, // 2
         { 0.0, +1.0, +1.0 }, // 3
         { +1.0, 0.0, 0.0 }, // 4
         { +1.0, 0.0, +1.0 }, // 5
         { +1.0, +1.0, 0.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7
      } ;
}

// -----------------------------------------------------------------------------------------------


EstrellaZ::EstrellaZ(int n)
:  MallaInd( "EstrellaZ" )
{
   const int GRADOS_CRIC = 360;
   assert(n>1);
   vertices = {{0.5, 0.5, 0}}; // El 0 es el centro
   col_ver.push_back({1.0,1.0,1.0});

   // converting degrees to radians
   float pasar_a_radian = M_PI/180;

   float x,y,
         sum_grados_interno = (GRADOS_CRIC/n)/2,
         grados_int_inicial = GRADOS_CRIC/n;
                                       // mientras que el angulo interno sea distinto al ultimo a generar  ( sum_grados_interno < (GRADOS_CRIC - (GRADOS_CRIC/n)/2) ) && 
   for(float grados = GRADOS_CRIC/n;grados <= GRADOS_CRIC; grados += grados_int_inicial)
   {                       // Puntas de la estrella
      
      x = (cos(grados*pasar_a_radian)/2) + 0.5;
      y = (sin(grados*pasar_a_radian)/2) + 0.5;

      std::cout << "P externo. X: " << x << "\t Y: " << y << "\n";

      Tupla3f tupla_insertar(x,y,0);
      vertices.push_back(tupla_insertar);

      //std::cout << "Posicion " << i <<" del vertices con coordenadas : "<<x<<" "<<y<<0<<"\n";

      col_ver.push_back(tupla_insertar);
                                              // Puntas internas de la estrella
      float x_interno = (cos(sum_grados_interno*pasar_a_radian)/4) + 0.5;
      float y_interno = (sin(sum_grados_interno*pasar_a_radian)/4)+ 0.5;

      Tupla3f tupla_insertar_interna(x_interno,y_interno ,0);
      vertices.push_back(tupla_insertar_interna);
      sum_grados_interno+=grados_int_inicial;
      col_ver.push_back(tupla_insertar_interna);

      std::cout << "P interno. X: " << x_interno << "\t Y: " << y_interno << "\n";


   }


   //for(int i = 0;i<vertices.size();i++)
      
   int pto_interno = 2;
   int pto_externo = 1;
   for(int i=0; i < n; i++){     
      for(int j=0; j < 2; j++){
         //std::cout << "Dibujamos triangulo :" << pto_interno << pto_externo << 0 << "\n";
         triangulos.push_back({pto_interno, pto_externo, 0});
         if(j==0 && (i!=(n-1)))
            pto_interno+=2;
         if(j==1 && (i==(n-1))){ //Para crear el último
            pto_interno = 2;
            triangulos.push_back({pto_interno, pto_externo, 0});
         }
      }
      pto_externo+=2;
   }



}


// -----------------------------------------------------------------------------------------------

CasaX::CasaX()
:  MallaInd( "cubo 8 vértices" )
{

   vertices =
      {  { 0.0, 0.0, 0.0 }, // 0
         { 0.0, 0.0, 1.0 }, // 1
         { 0.0, 0.5, 1.0 }, // 2
         { 0.0, 0.5, 0.0 }, // 3
         { +1.0, 0.0, 0.0 }, // 4
         { +1.0, 0.0, 1.0 }, // 5
         { +1.0, 0.5, 0.0 }, // 6
         { +1.0, 0.5, 1.0 }, // 7
         { +1.0, +1.0, +0.5}, // 8 esquina positiva en x
         { 0.0, +1.0, +0.5}  // 9 esquina negativa en x
      } ;



   triangulos =
      {  {0,1,2}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

         {0,6,4}, {0,3,6}, // Z-
         {1,5,7}, {1,7,2},  // Z+ (+1)

         {6,7,8}, {2,3,9}, // caras frontales del tejado

         {2,7,8}, {2,8,9},
         {3,6,8}, {3,8,9}

      } ;

      col_ver =
      {  { 0.0, 0.0, 0.0 }, // 0
         { 0.0, 0.0, 1.0 }, // 1
         { 0.0, 0.5, 1.0 }, // 2
         { 0.0, 0.5, 0.0 }, // 3
         { +1.0, 0.0, 0.0 }, // 4
         { +1.0, 0.0, 1.0 }, // 5
         { +1.0, 0.5, 0.0 }, // 6
         { +1.0, 0.5, 1.0 }, // 7
         { +1.0, +1.0, +0.5}, // 8 esquina positiva en x
         { 0.0, +1.0, +0.5}  // 9 esquina negativa en x
      } ;

}

// -----------------------------------------------------------------------------------------------

PiramideEstrellaZ::PiramideEstrellaZ(int n)
:  MallaInd( "EstrellaZ" )
{
   const int GRADOS_CRIC = 360;
   assert(n>1);
   vertices = {{0.5, 0.5, 0}}; // El 0 es el centro
   col_ver.push_back({1.0,1.0,1.0});

   // converting degrees to radians
   float pasar_a_radian = M_PI/180;

   float x,y,
         sum_grados_interno = (GRADOS_CRIC/n)/2,
         grados_int_inicial = GRADOS_CRIC/n;
                                       // mientras que el angulo interno sea distinto al ultimo a generar  ( sum_grados_interno < (GRADOS_CRIC - (GRADOS_CRIC/n)/2) ) && 
   for(float grados = GRADOS_CRIC/n;grados <= GRADOS_CRIC; grados += grados_int_inicial)
   {                       // Puntas de la estrella
      
      x = (cos(grados*pasar_a_radian)/2) + 0.5;
      y = (sin(grados*pasar_a_radian)/2) + 0.5;

      std::cout << "P externo. X: " << x << "\t Y: " << y << "\n";


      Tupla3f tupla_insertar(x,y,0);
      vertices.push_back(tupla_insertar);

      //std::cout << "Posicion " << i <<" del vertices con coordenadas : "<<x<<" "<<y<<0<<"\n";

      col_ver.push_back(tupla_insertar);
                                              // Puntas internas de la estrella
      float x_interno = (cos(sum_grados_interno*pasar_a_radian)/4) + 0.5;
      float y_interno = (sin(sum_grados_interno*pasar_a_radian)/4)+ 0.5;

      Tupla3f tupla_insertar_interna(x_interno,y_interno ,0);
      vertices.push_back(tupla_insertar_interna);
      sum_grados_interno+=grados_int_inicial;
      col_ver.push_back(tupla_insertar_interna);

      std::cout << "P interno. X: " << x_interno << "\t Y: " << y_interno << "\n";


   }

   // Introducimos el punto del centro desplazado para poder unirlo posteriormente
   vertices.push_back({0.5,0.5,0.5});

   int pto_interno = 2;
   int pto_externo = 1;
   for(int i=0; i < n; i++){     
      for(int j=0; j < 2; j++){
         triangulos.push_back({pto_interno, pto_externo, 0});
         // Unimos con el nuevo centro
         triangulos.push_back({pto_interno, pto_externo, vertices.size()-1});
         if(j==0 && (i!=(n-1)))
            pto_interno+=2;
         if(j==1 && (i==(n-1))){ //Para crear el último
            pto_interno = 2;
            triangulos.push_back({pto_interno, pto_externo, 0});
            // Unimos con el nuevo centro
            triangulos.push_back({pto_interno, pto_externo, vertices.size()-1});
         }
      }
      pto_externo+=2;
   }


}

// -----------------------------------------------------------------------------------------------

RejillaY::RejillaY(unsigned int n, unsigned int m)
{
   // Comprobamos las condiciones necesarias
   assert(n>1 && m > 1);

   // Creamos las variables locales
   float  x_valor=0.0,z_valor=0.0, // Valores a los que se les va sumando los incrementos
          incremento_x = 1.0/ (n-1),
          incremento_z = 1.0/ (m-1);

   // Creamos la malla de vertices
   for(int i = 0;i<m;i++)
   {
      for(int j=0;j<n;j++)
      {
         vertices.push_back({x_valor,0.0,z_valor});
         col_ver.push_back({x_valor,0.0,z_valor});
         x_valor+=incremento_x;
         
      }
      z_valor+=incremento_z;
      x_valor=0.0;
   }
   // Creaos la tabla de triangulos

   for(int i = 0;i<m-1;i++)
   {
      for(int j=0;j<n;j++)
      {
         // Si estamos en un vertice par 
         if(j == 0)
            triangulos.push_back({(i*m)+j,(i*m)+j+1,(i*m)+j+m});

         // Si estamos en un vertice impar 
         else if( j == n-1)
            triangulos.push_back({(i*m)+j,(i*m)+j+m-1,(i*m)+j+m});
         else 
         {
            triangulos.push_back({(i*m)+j,(i*m)+j+1,(i*m)+j+m});
            triangulos.push_back({(i*m)+j,(i*m)+j+m-1,(i*m)+j+m});
         }
      }
      
   }
}

// -----------------------------------------------------------------------------------------------

MallaTorre::MallaTorre(int n)
{
   // Creamos la tabla de vertices
   for(int i = 0;i<n;i++)
   {
         // Los de la base solo hace falta que se pongan al principio
         // luego basta con ir añadiendo los dos de arriba cada vez
         
         vertices.push_back({0.5,0.0+i,0.5});// 0
         vertices.push_back({0.5,0.0+i,-0.5});// 1
         
         vertices.push_back({0.5,1.0+i,0.5});// 2
         vertices.push_back({0.5,1.0+i,-0.5});// 3

         
         vertices.push_back({-0.5,0.0+i,0.5});// 4
         vertices.push_back({-0.5,0.0+i,-0.5});// 5
         
         vertices.push_back({-0.5,1.0+i,0.5});// 6
         vertices.push_back({-0.5,1.0+i,-0.5});// 7
   }

   // Creamos la tabla de triangulos
   for(int i = 0;i<n;i++)
   {
      triangulos.push_back({0+(8*i),1+(8*i),2+(8*i)});
      triangulos.push_back({1+(8*i),2+(8*i),3+(8*i)});

      triangulos.push_back({1+(8*i),3+(8*i),5+(8*i)});
      triangulos.push_back({3+(8*i),5+(8*i),7+(8*i)});
      
      triangulos.push_back({4+(8*i),5+(8*i),6+(8*i)});
      triangulos.push_back({5+(8*i),6+(8*i),7+(8*i)});
      
      triangulos.push_back({0+(8*i),2+(8*i),4+(8*i)});
      triangulos.push_back({2+(8*i),4+(8*i),6+(8*i)});
   }
}
// -----------------------------------------------------------------------------------------------
MallaPiramideL::MallaPiramideL()
:  MallaInd( "Malla piramide" )
{

   vertices =
      {  
         { 0.0, 0.0, 0.0}, // 0
         { 0.0, 0.0, -1.0}, // 1
         { 1.0, 0.0, -1.0}, // 2
         { 1.0, 0.0, 1.0 }, // 3
         { -1.0, 0.0, 1.0 }, // 4
         { -1.0, 0.0, 0.0}, // 5
         { 0.0, 2.0, 0.0} // 6
      } ;

   triangulos =
      {  {0,1,2}, {0,4,5}, {0,2,3}, {0,3,4},
         {6,0,1}, {6,1,2}, {6,2,3}, {6,3,4},
         {6,4,5}, {6,5,0}

      } ;

}
// Ejercicio tipo examen

Diamante::Diamante(): MallaInd( "Diamante SIMS " )
{

   vertices.clear();

   // Base
   vertices.push_back({0.5, 0, 0.5}); // 0
   vertices.push_back({0.5, 0, -0.5});  // 1
   vertices.push_back({-0.5, 0, -0.5});   // 2
   vertices.push_back({-0.5, 0, 0.5}); // 3

   // Picos
   vertices.push_back({0.0, 1, 0.0}); // 4
   vertices.push_back({0.0, -1, 0.0}); // 5


   triangulos =
      {  
         {0, 1, 4}, {1, 2, 4}, {2, 3, 4}, {0, 3, 4},  // Parte de arriba

         {0, 1, 5}, {1, 2, 5}, {2, 3, 5}, {0, 3, 5}   // Parte de abajo

      } ;

   
   for(int i=0; i<vertices.size(); i++){
      col_ver.push_back({0,1,0});
   }

}
// -----------------------------------------------------------------------------------------------

CuboEstrella::CuboEstrella()
:  MallaInd( "CuboEstrella" )
{

   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0             
         { -1.0, -1.0, +1.0 }, // 1                
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7
         {-6.0,0.0,0.0},   //8
         {6.0,0.0,0.0},    //9
         {0.0,0.0,-6.0},   //10
         {0.0,0.0,6.0}    //11
      } ;



   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

         {0,5,1}, {0,4,5}, // Y-
         {2,3,7}, {2,7,6}, // Y+ (+2)

         {0,6,4}, {0,2,6}, // Z-
         {1,5,7}, {1,7,3},  // Z+ (+1)

         {3,2,8},{1,0,8},{3,1,8},{2,0,8},
         {3,7,11},{3,1,11},{7,5,11},{1,5,11},
         {7,6,9},{7,5,9},{5,4,9},{4,6,9},
         {2,6,10},{2,0,10},{0,4,10},{4,6,10}
      } ;

   for(int i=0; i < vertices.size(); i++){
      Tupla3f color;
      color(X)=(vertices.at(i)(X)+6.0)/12;
      color(Y)=(vertices.at(i)(Y)+6.0)/12;
      color(Z)=(vertices.at(i)(Z)+6.0)/12;

      col_ver.push_back(color);
   }

}

// ---------------------------------------- Ejercicio adicional P4-------------------------------------------------------

MallaDiscoP4::MallaDiscoP4()
{
   ponerColor({1.0, 1.0, 1.0});
   const unsigned ni = 23, nj = 31 ;
   for( unsigned i= 0 ; i < ni ; i++ )
      for( unsigned j= 0 ; j < nj ; j++ )
      {
         const float
         fi = float(i)/float(ni-1),
         fj = float(j)/float(nj-1),
         ai = 2.0*M_PI*fi,
         x = fj * cos( ai ),
         y = fj * sin( ai ),
         z = 0.0 ;
         vertices.push_back({ x, y, z });
         //cc_tt_ver.push_back({(x + 1.0)/(2), (y + 1.0 )/(2)});
         cc_tt_ver.push_back({fi,fj}); // Ejercicio 2
      }

   for( unsigned i= 0 ; i < ni-1 ; i++ )
      for( unsigned j= 0 ; j < nj-1 ; j++ )
      {
         triangulos.push_back({ i*nj+j, i*nj+(j+1), (i+1)*nj+(j+1) });
         triangulos.push_back({ i*nj+j, (i+1)*nj+(j+1), (i+1)*nj+j });
      }
}
// -----------------------------------------------------------------------------------------------

MallaEXP4::MallaEXP4()
{
      vertices =
      {  { 1.0, 0.0, 0.0 }, // 0
         { 1.0, 0.0, 0.0 }, // 0,1
         { 1.0, 0.0, 0.0 }, // 0,2

         { 0.0, 1.0, 0.0 }, // 1,3
         { 0.0, 1.0, 0.0 }, // 1,4
         { 0.0, 1.0, 0.0 }, // 1,5

         { 0.0, 0.0, 1.0 }, // 2,6
         { 0.0, 0.0, 1.0 }, // 2,7
         { 0.0, 0.0, 1.0 }, // 2,8
         
         { 0.0, 0.0, 0.0 }, // 3,9
         { 0.0, 0.0, 0.0 }, // 3,10
         { 0.0, 0.0, 0.0 }, // 3,11

      } ;



   triangulos =
      {  {9,3,0}, {1,4,6},
         {10,2,7}, {11,8,5}
      } ;

   cc_tt_ver = 
      {
         {0.0,1.0},//0
         {1.0,1.0},
         {1.0,1.0},
         {0.5,0.0},//1
         {0.5,0.0},
         {0.5,0.0},
         {0.0,1.0},//2
         {0.0,1.0},
         {1.0,1.0},
         {1.0,1.0},//3
         {0.5,0.0},
         {0.0,1.0},
      };
   calcularNormales();
}
