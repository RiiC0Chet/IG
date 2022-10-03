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

}


// -----------------------------------------------------------------------------
// calcula las dos tablas de normales

void MallaInd::calcularNormales()
{
   // COMPLETAR: en la práctica 4: calculo de las normales de la malla
   // se debe invocar en primer lugar 'calcularNormalesTriangulos'
   // .......


}



// --------------------------------------------------------------------------------------------

void MallaInd::visualizarGL( ContextoVis & cv )
{
   using namespace std ;
   assert( cv.cauce != nullptr );
   CError();

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
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

         {0,5,1}, {0,4,5}, // Y-
         {2,3,7}, {2,7,6}, // Y+ (+2)

         {0,6,4}, {0,2,6}, // Z-
         {1,5,7}, {1,7,3}  // Z+ (+1)
      } ;

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
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

         {0,5,1}, {0,4,5}, // Y-
         //{2,3,7}, {2,7,6}, // Y+ (+2)   // quito cara superior
         {2,3,8}, {3,7,8}, {7,6,8}, {6,2,8}, // añado tejado 

         {0,6,4}, {0,2,6}, // Z-
         {1,5,7}, {1,7,3}  // Z+ (+1)
      } ;

}

// -----------------------------------------------------------------------------------------------

Tetraedro::Tetraedro()
:  MallaInd( "Tetraedro 4 vértices" )
{

   vertices =
      {  { -1.0, -1.0, -1.0 }, // 1
         { -1.0, -1.0, +1.0 }, // 2
         { -1.0, +1.0, -1.0 }, // 3 
      } ;



   triangulos =
      {  
         {0,1,3}, {0,1,2}, {0,2,3}, {1,2,3}
      } ;
      ponerColor({0.0,0.0,0.0});

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

         //{0,5,1}, {0,4,5}, // Y-
         //{2,3,7}, {2,7,6}, // Y+ (+2)

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
//beethoven::beethoven(const std::string & nombre_arch) : MallaPLY(nombre_arch){}
// -----------------------------------------------------------------------------------------------
