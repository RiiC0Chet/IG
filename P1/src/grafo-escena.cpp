// Nombre: Jose Luis Apellidos: Rico Ramos Titulación: GIADE
// email: jlricoramos@correo.ugr.es DNI o pasaporte: 20080849D

// *********************************************************************
// **
// ** Gestión de una grafo de escena (implementación)
// ** Copyright (C) 2016 Carlos Ureña
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

#include "ig-aux.h"
#include "grafo-escena.h"
#include "malla-ind.h"
#include "malla-revol.h"

using namespace std ;

// *********************************************************************
// Entrada del nodo del Grafo de Escena

// ---------------------------------------------------------------------
// Constructor para entrada de tipo sub-objeto

EntradaNGE::EntradaNGE( Objeto3D * pObjeto )
{
   assert( pObjeto != NULL );
   tipo   = TipoEntNGE::objeto ;
   objeto = pObjeto ;
}
// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( const Matriz4f & pMatriz )
{
   tipo    = TipoEntNGE::transformacion ;
   matriz  = new Matriz4f() ; // matriz en el heap, puntero propietario
   *matriz = pMatriz ;
}

// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( Material * pMaterial )
{
   assert( pMaterial != NULL );
   tipo     = TipoEntNGE::material ;
   material = pMaterial ;
}

// -----------------------------------------------------------------------------
// Destructor de una entrada

EntradaNGE::~EntradaNGE()
{
   /**  no fnciona debido a que se hacen copias (duplicados) de punteros
   if ( tipo == TipoEntNGE::transformacion )
   {
      assert( matriz != NULL );
      delete matriz ;
      matriz = NULL ;
   }
   * **/
}

// *****************************************************************************
// Nodo del grafo de escena: contiene una lista de entradas
// *****************************************************************************

// -----------------------------------------------------------------------------
// Visualiza usando OpenGL

void NodoGrafoEscena::visualizarGL( ContextoVis & cv )
{
   // COMPLETAR: práctica 3: recorrer las entradas y visualizar cada nodo.
   // ........

   //copiamos al material previo 
   Material * material_pre = cv.material_act;
   // guarda modelview actual
   cv.cauce->pushMM();

   // HAY QUE COMPROBAR COLOR Y RESTAURAR AL FINAL ETC
   // guardar el color previamente fijado y fijar el color del objeto actual
   const Tupla4f color_previo = leerFijarColVertsCauce( cv );

   // recorrer todas las entradas del array que hay en el nodo:
   for( unsigned i = 0 ; i < entradas.size() ; i++ )
   {
      switch( entradas[i].tipo )
      { 
         case TipoEntNGE::objeto : // entrada objeto:
            entradas[i].objeto->visualizarGL( cv ); // visualizar objeto
         break ;
         case TipoEntNGE::transformacion : // entrada transf.:
            cv.cauce->compMM( *(entradas[i].matriz)); // componer matriz
         break ;
         case TipoEntNGE::material :
            if(cv.iluminacion) entradas[i].material->activar(cv);
         break;
      }
   }
   
   // restaurar el color previamente fijado
   cv.cauce->fijarColor( color_previo );

   // restaura modelview guardada
   cv.cauce->popMM() ;

   // si el material previo no es nulo, lo copiamos y lo activamos
   if(material_pre != nullptr)
   {
      cv.material_act = material_pre;
      cv.material_act->activar( cv );
   }

   // COMPLETAR: práctica 4: en la práctica 4, si 'cv.iluminacion' es 'true',
   // se deben de gestionar los materiales:
   //   1. guardar puntero al material activo al inicio (está en cv.material_act)
   //   2. si una entrada des de tipo material, activarlo y actualizar 'cv.material_act'
   //   3. al finalizar, restaurar el material activo al inicio (si es distinto del actual)



}




// *****************************************************************************
// visualizar pura y simplemente la geometría, sin colores, normales, coord. text. etc...

void NodoGrafoEscena::visualizarGeomGL( ContextoVis & cv )
{
   // comprobar que hay un cauce en 'cv' 
   assert( cv.cauce != nullptr );
  

   // COMPLETAR: práctica 3
   //
   // Este método hace un recorrido de las entradas del nodo, parecido a 'visualizarGL', teniendo 
   // en cuenta estos puntos:
   //
   // - usar push/pop de la matriz de modelado al inicio/fin (al igual que en visualizatGL)
   // - recorrer las entradas, llamando recursivamente a 'visualizarGeomGL' en los nodos u objetos hijos
   // - ignorar el color o identificador del nodo (se supone que el color ya está prefijado antes de la llamada)
   // - ignorar las entradas de tipo material, y la gestión de materiales (se usa sin iluminación)

   // .......

   // guarda modelview actual
   cv.cauce->pushMM();


   // recorrer todas las entradas del array que hay en el nodo:
   for( unsigned i = 0 ; i < entradas.size() ; i++ )
   {
      switch( entradas[i].tipo )
      { 
         case TipoEntNGE::objeto : // entrada objeto:
            entradas[i].objeto->visualizarGeomGL( cv ); // visualizar objeto
         break ;
         case TipoEntNGE::transformacion : // entrada transf.:
            cv.cauce->compMM( *(entradas[i].matriz)); // componer matriz
         break ;
      }
   }
   
   // restaura modelview guardada
   cv.cauce->popMM() ;

}



// -----------------------------------------------------------------------------

NodoGrafoEscena::NodoGrafoEscena()
{

}

// -----------------------------------------------------------------------------
// Añadir una entrada (al final).
// genérica (de cualqiuer tipo de entrada)

unsigned NodoGrafoEscena::agregar( const EntradaNGE & entrada )
{
   // COMPLETAR: práctica 3: agregar la entrada al nodo, devolver índice de la entrada agregada
   // ........
   entradas.push_back(entrada);
   return entradas.size()-1 ; // sustituir por lo que corresponda ....

}
// -----------------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// objeto (copia solo puntero)

unsigned NodoGrafoEscena::agregar( Objeto3D * pObjeto )
{
   return agregar( EntradaNGE( pObjeto ) );
}
// ---------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// matriz (copia objeto)

unsigned NodoGrafoEscena::agregar( const Matriz4f & pMatriz )
{
   return agregar( EntradaNGE( pMatriz ) );
}
// ---------------------------------------------------------------------
// material (copia solo puntero)
unsigned NodoGrafoEscena::agregar( Material * pMaterial )
{
   return agregar( EntradaNGE( pMaterial ) );
}

// devuelve el puntero a la matriz en la i-ésima entrada
Matriz4f * NodoGrafoEscena::leerPtrMatriz( unsigned indice )
{
   // COMPLETAR: práctica 3: devolver puntero la matriz en ese índice
   //   (debe de dar error y abortar si no hay una matriz en esa entrada)
   // ........(sustituir 'return nullptr' por lo que corresponda)

   assert( indice < entradas.size() );
   assert( entradas[indice].tipo == TipoEntNGE::transformacion );
   assert( entradas[indice].matriz != nullptr );
   return entradas[indice].matriz ;
}
// -----------------------------------------------------------------------------
// si 'centro_calculado' es 'false', recalcula el centro usando los centros
// de los hijos (el punto medio de la caja englobante de los centros de hijos)

void NodoGrafoEscena::calcularCentroOC()
{

   // COMPLETAR: práctica 5: calcular y guardar el centro del nodo
   //    en coordenadas de objeto (hay que hacerlo recursivamente)
   //   (si el centro ya ha sido calculado, no volver a hacerlo)
   // ........
   if(centro_calculado) return;

   int num_hijos = 0;
   Matriz4f mmodelado = MAT_Ident();
   Tupla3f centro_aux = {0.0,0.0,0.0};

   for(uint i = 0; i < entradas.size(); i++){
      if(entradas[i].tipo == TipoEntNGE::transformacion ){
         mmodelado = mmodelado * (*entradas[i].matriz);
      }
      if(entradas[i].tipo == TipoEntNGE::objeto ){
         entradas[i].objeto->calcularCentroOC();
         centro_aux = centro_aux + (mmodelado * entradas[i].objeto->leerCentroOC());
         num_hijos++;
      }
   }
   ponerCentroOC(centro_aux/float(num_hijos));
   centro_calculado = true;

}
// -----------------------------------------------------------------------------
// método para buscar un objeto con un identificador y devolver un puntero al mismo

bool NodoGrafoEscena::buscarObjeto
(
   const int         ident_busc, // identificador a buscar
   const Matriz4f &  mmodelado,  // matriz de modelado
   Objeto3D       ** objeto,     // (salida) puntero al puntero al objeto
   Tupla3f &         centro_wc   // (salida) centro del objeto en coordenadas del mundo
)
{
   assert( 0 < ident_busc );

// COMPLETAR: práctica 5: buscar un sub-objeto con un identificador
   // Se deben de dar estos pasos:

   Matriz4f mmodelado_aux = mmodelado;

   // 1. calcula el centro del objeto, (solo la primera vez)
   // ........
   calcularCentroOC();

   // 2. si el identificador del nodo es el que se busca, ya está (terminar)
   // ........
   if(leerIdentificador() == ident_busc){
      *objeto = this;
      centro_wc = mmodelado*leerCentroOC();
      return true;
   }
   // 3. El nodo no es el buscado: buscar recursivamente en los hijos
   //    (si alguna llamada para un sub-árbol lo encuentra, terminar y devolver 'true')
   // ........
   else{
      for(uint i = 0; i < entradas.size(); i++){
         if(entradas[i].tipo == TipoEntNGE::transformacion ){
            mmodelado_aux = mmodelado_aux * (*entradas[i].matriz);
         }
         if(entradas[i].tipo == TipoEntNGE::objeto){
            if(entradas[i].objeto->buscarObjeto(ident_busc, mmodelado_aux, objeto, centro_wc))
               return true;
         }
      }
   }
   // ni este nodo ni ningún hijo es el buscado: terminar
   return false ;
}


//---------------------------------------------------------------------------------------

GrafoEstrellaX::GrafoEstrellaX(int n, float alpha)
{
   cout<<"---------------------------------------------------------------------------------------"<<endl;
   assert(n>1);
   int indice_1 = agregar(MAT_Rotacion(alpha,{1.0,0.0,0.0}));
   agregar(MAT_Rotacion( 90, { 0.0, 1.0, 0.0 } ) ) ;
   
   
   agregar(MAT_Traslacion({-0.5,-0.5,0.0}));
   agregar(MAT_Escalado(2.5,2.6,1.0));
   agregar(MAT_Traslacion({-0.3,-0.3,0.0}));
   agregar(new EstrellaZ(n));

   // Ahora isnertamos los conos en las esquinas
   // Invertimos todos los cambios hechos antes para la estrella 
   agregar(MAT_Traslacion({0.3,0.3,0.0}));
   agregar(MAT_Escalado(0.4,0.384,1.0));
   agregar(MAT_Traslacion({0.5,0.5,0.0}));


   const int GRADOS_CRIC = 360;

   // converting degrees to radians
   float pasar_a_radian = M_PI/180;

   float x,y,
         grados_int_inicial = GRADOS_CRIC/n;
                                       // mientras que el angulo interno sea distinto al ultimo a generar  ( sum_grados_interno < (GRADOS_CRIC - (GRADOS_CRIC/n)/2) ) && 
   
   for(float grados = GRADOS_CRIC/n;grados <= GRADOS_CRIC; grados += grados_int_inicial)
   {                       // Puntas de la estrella
      
      
      x = (cos(grados*pasar_a_radian)*1.3);
      y = (sin(grados*pasar_a_radian)*1.3);

      std::cout << "Cono asociado con grados: " << x <<" "<<grados << "\n";
      
      agregar(new ConoX(x,y, grados));

      
   }

   pm_rot = leerPtrMatriz(indice_1);

}

unsigned GrafoEstrellaX::leerNumParametros() const
{
   return 1;
}

void GrafoEstrellaX::actualizarEstadoParametro( const unsigned iParam, const float t_sec )
{
   assert(iParam < leerNumParametros() && iParam >=0);

    switch(iParam)
    {
        case 0:
            // da 2.5 vueltas por segudno
            *pm_rot = MAT_Rotacion( 360*t_sec*2.5, { 1.0, 0.0, 0.0 } ) ;
        break;
    }
}
//---------------------------------------------------------------------------------------
ConoX::ConoX(float x, float y, float grados)
{
   agregar(MAT_Traslacion({x,y,0.0}));
   agregar(MAT_Rotacion(grados-90,{0.0,0.0,1.0}));
   // Aplicamos el escalado
   agregar(MAT_Escalado(0.14,0.15,0.14));
   agregar(new Cono(10,20));
}
//---------------------------------------------------------------------------------------
GrafoCuboX::GrafoCuboX()
{
   //Insertamos los cubitos chicos
   cubo_1 = new GrafoCubos({1.0,0.0,0.0});
   agregar(cubo_1);

   cubo_2 = new GrafoCubos({-1.0,0.0,0.0});
   agregar(cubo_2);

   cubo_3 = new GrafoCubos({0.0,1.0,0.0});
   agregar(cubo_3);

   cubo_4 = new GrafoCubos({0.0,-1.0,0.0});
   agregar(cubo_4);

   cubo_5 = new GrafoCubos({0.0,0.0,1.0});
   agregar(cubo_5);

   cubo_6 = new GrafoCubos({0.0,0.0,-1.0});
   agregar(cubo_6);


   //Empezamos a formar el cubo
   agregar(MAT_Traslacion({-0.5,-0.5,-0.5}));

   agregar(new RejillaY(5,5));
   agregar(MAT_Traslacion({0.0,1.0,0.0}));
   agregar(new RejillaY(5,5));
   agregar(MAT_Traslacion({0.0,-1.0,0.0}));

   agregar(MAT_Rotacion(90,{0.0,0.0,1.0}));
   agregar(new RejillaY(5,5));
   agregar(MAT_Traslacion({0.0,-1.0,0.0}));
   agregar(new RejillaY(5,5));
   agregar(MAT_Traslacion({0.0,1.0,0.0}));
   agregar(MAT_Rotacion(360-90,{0.0,0.0,1.0}));

   agregar(MAT_Rotacion(90,{-1.0,0.0,0.0}));
   agregar(new RejillaY(5,5));
   agregar(MAT_Traslacion({0.0,-1.0,0.0}));
   agregar(new RejillaY(5,5));
}

unsigned GrafoCuboX::leerNumParametros() const
{
   return 1;
}

void GrafoCuboX::actualizarEstadoParametro( const unsigned iParam, const float t_sec )
{
   assert(iParam < leerNumParametros() && iParam >=0);

    switch(iParam)
    {
        case 0:
            cubo_1->actualizarEstadoParametro(iParam,t_sec);
            cubo_2->actualizarEstadoParametro(iParam,t_sec);
            cubo_3->actualizarEstadoParametro(iParam,t_sec);
            cubo_4->actualizarEstadoParametro(iParam,t_sec);
            cubo_5->actualizarEstadoParametro(iParam,t_sec);
            cubo_6->actualizarEstadoParametro(iParam,t_sec);
        break;

    }
}



GrafoCubos::GrafoCubos(Tupla3f cara)
{
   int indice_rot = agregar(MAT_Rotacion(0.0,{1.0,0.0,0.0}));
   agregar(MAT_Escalado(0.1,0.1,0.1));

   if(cara(0) == 1.0)
   {
      cara_ = 0;
      agregar(MAT_Traslacion({0.62/0.1,0.0,0.0}));
      agregar(MAT_Escalado(1.0*1.5,1.0,1.0));
   }
   else if(cara(0) == -1.0)
   {
      cara_ = 0;
      agregar(MAT_Traslacion({-0.62/0.1,0.0,0.0}));
      agregar(MAT_Escalado(1.0*1.5,1.0,1.0));
   }
   else if(cara(1) == 1.0)
   {
      cara_ = 1;
      agregar(MAT_Traslacion({0.0,0.62/0.1,0.0}));
      agregar(MAT_Escalado(1.0,1.0*1.5,1.0));
   }
   else if(cara(1) == -1.0)
   {
      cara_ = 1;
      agregar(MAT_Traslacion({0.0,-0.62/0.1,0.0}));
      agregar(MAT_Escalado(1.0,1.0*1.5,1.0));
   }
   else if(cara(2) == 1.0)
   {
      cara_ = 2;
      agregar(MAT_Traslacion({0.0,0.0,0.62/0.1}));
      agregar(MAT_Escalado(1.0,1.0,1.0*1.5));
   }
   else
   {
      cara_ = 2;
      agregar(MAT_Traslacion({0.0,0.0,-0.62/0.1}));
      agregar(MAT_Escalado(1.0,1.0,1.0*1.5));
   }
   
   agregar(new Cubo());

   pm_rot = leerPtrMatriz(indice_rot);
}

unsigned GrafoCubos::leerNumParametros() const
{
   return 1;
}

void GrafoCubos::actualizarEstadoParametro( const unsigned iParam, const float t_sec )
{
   assert(iParam < leerNumParametros() && iParam >=0);

    switch(iParam)
    {
        case 0:
            if(cara_ == 0)
            {
               *pm_rot = MAT_Rotacion( 360*t_sec, { 1.0, 0.0, 0.0 } ) ;
            }
            else if(cara_ == 1)
            {
               *pm_rot = MAT_Rotacion( 360*t_sec, { 0.0, 1.0, 0.0 } ) ;
            }
            else
            {
               *pm_rot = MAT_Rotacion( 360*t_sec, { 0.0, 0.0, 1.0 } ) ;
            }
            break;
    }
}


//---------------------------------------------------------------------------------------
NodoCubo24::NodoCubo24()
{
   
}

//---------------------------------------------------------------------------------------

NodoDiscoP4::NodoDiscoP4()
{
   Textura * tex = new Textura("cuadricula.jpg");
   Material * mat = new Material(tex,0.8,0.8,1.0,1.0);

   ponerNombre("Nodo ejercicio adicional práctica 4, examen 27 enero");

   agregar(mat);
   agregar( new MallaDiscoP4() );
}

//---------------------------------------------------------------------------------------
GrafoEsferasP5::GrafoEsferasP5()
{
   const unsigned n_filas_esferas = 8,
                  n_esferas_x_fila = 5 ;

   const float e = 0.4/n_esferas_x_fila ;
   agregar( MAT_Escalado( e,e,e ));

   for( unsigned i = 0 ; i < n_filas_esferas ; i++ )
   {
      NodoGrafoEscena * fila_esferas = new NodoGrafoEscena() ;

      for( unsigned j = 0 ; j < n_esferas_x_fila ; j++ )
      {
         Esfera * esfera = new Esfera(15,15) ;
         fila_esferas->agregar( MAT_Traslacion( {2.2, 0.0, 0.0 }));
         esfera->ponerIdentificador(((i*n_esferas_x_fila)+j)+1);
         fila_esferas->agregar( esfera );
      }
      agregar( fila_esferas );
      agregar( MAT_Traslacion( {0.0, 0.0, 5.0 }));
      
   }
}
//---------------------------------------------------------------------------------------

GrafoEsferasP5_2::GrafoEsferasP5_2()
{
   const unsigned n_filas_esferas = 16,
                  n_esferas_x_fila = 1 ;
   const float e = 2.5/n_esferas_x_fila ;

   agregar( MAT_Escalado( e, e, e ));

   for( unsigned i = 0 ; i < n_filas_esferas ; i++ )
   {
      NodoGrafoEscena * fila_esferas = new NodoGrafoEscena() ;
      fila_esferas->agregar( MAT_Traslacion( {1.0, 0.0, 0.0 }));
      
      // reducimos el radio
      fila_esferas->agregar(MAT_Escalado(M_PI/n_filas_esferas,M_PI/n_filas_esferas,M_PI/n_filas_esferas));

      for( unsigned j = 0 ; j < n_esferas_x_fila ; j++ )
      {
         Esfera * esfera = new Esfera(15,20) ;

         
         fila_esferas->agregar( MAT_Traslacion({ 2.5, 0.0, 0.0 }));
         esfera->ponerIdentificador(((i*n_esferas_x_fila)+j)+1);
         fila_esferas->agregar( esfera );
      }

      agregar( fila_esferas );
      agregar( MAT_Rotacion( 360.0/n_filas_esferas, { 0.0, 1.0, 0.0 }));
   }
}

//---------------------------------------------------------------------------------------

EsferaEXP5::EsferaEXP5(int n)
{
   // reducimos el radio
   agregar(MAT_Escalado(M_PI/n,M_PI/n,M_PI/n));
   agregar(new Esfera(15,20));
}


AnilloEXP5::AnilloEXP5(int n)
{
   assert(n>3);

   const unsigned n_esferas_x_fila = 1 ;

   for( unsigned i = 0 ; i < n ; i++ )
   {
      NodoGrafoEscena * fila_esferas = new NodoGrafoEscena() ;
      fila_esferas->agregar( MAT_Traslacion( {1.0, 0.0, 0.0 }));

      for( unsigned j = 0 ; j < n_esferas_x_fila ; j++ )
      {
         EsferaEXP5 * esfera = new EsferaEXP5(n) ;

         esfera->ponerIdentificador(i+1);
         fila_esferas->agregar( esfera );
      }

      agregar( fila_esferas );
      agregar( MAT_Rotacion( 360.0/n, { 0.0, 1.0, 0.0 }));
   }

}


NodoEXP4::NodoEXP4()
{
   Textura * tex = new Textura("textura-exp4-v2.jpg");
   Material * mat = new Material (tex,0.5,0.5,0.5,80);

   agregar(mat);
   agregar(new MallaEXP4());
}
