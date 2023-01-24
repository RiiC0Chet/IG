// Nombre: Jose Luis Apellidos: Rico Ramos Titulación: GIADE
// email: jlricoramos@correo.ugr.es DNI o pasaporte: 20080849D

#include "ig-aux.h"
#include "practicas.h"
#include "camara.h"
#include "fbo.h"
#include "seleccion.h"
#include "grafo-escena.h"

// framebuffer object usado para selección
static Framebuffer * fbo = nullptr ;

// ---------------------------------------------------------------------

Framebuffer * FBOActual()
{
   return fbo ;
}

// ---------------------------------------------------------------------
// fijar el color actual en el cauce usando un identificador entero

void FijarColVertsIdent( Cauce & cauce, const int ident )  // 0 ≤ ident < 2^24
{
   // COMPLETAR: práctica 5: fijar color actual de OpenGL usando 'ident' (con 'cauce.fijarColor')
   // .....
   const unsigned char
      byteR = (ident           ) % 0x100U, // rojo = byte menos significativo
      byteG = (ident /   0x100U) % 0x100U, // verde = byte intermedio
      byteB = (ident / 0x10000U) % 0x100U; // azul = byte más significativo

      std::cout << "Rojo: " << byteR << " Verde: " << byteG << " Azul" << byteB << std::endl;

      cauce.fijarColor( float(byteR)/255.0f, float(byteG)/255.0f, float(byteB)/255.0f);
}

// ----------------------------------------------------------------------------------
// leer un identificador entero codificado en el color de un pixel en el
// framebuffer activo actualmente

int LeerIdentEnPixel( int xpix, int ypix )
{
   // COMPLETAR: práctica 5: leer el identificador codificado en el color del pixel (x,y)
   // .....(sustituir el 'return 0' por lo que corresponda)
   // .....
   unsigned char bytes[3] ; // para guardar los tres bytes
   // leer los 3 bytes del frame-buffer
   glReadPixels( xpix,ypix, 1,1, GL_RGB,GL_UNSIGNED_BYTE, (void *)bytes);
   // reconstruir el indentificador y devolverlo:
   std::cout << "Rojo: " << bytes[0] << " Verde: " << 0x100U*bytes[1] << " Azul" << 0x10000U*bytes[2] << std::endl;
   return bytes[0] + ( 0x100U*bytes[1] ) + ( 0x10000U*bytes[2] ) ;

}
// -------------------------------------------------------------------------------
// Función principal de selección, se llama al hacer click con el botón izquierdo
//
// (x,y)       = posición donde se ha hecho click en coordenadas del sistema de ventanas (enteras)
// objeto_raiz = objeto raiz (no subobjeto de otro) que se está visualizando en la ventana
// cv_dib      = contexto de visualización usado para dibujar el objeto
//
// devuelve: true si se ha seleccionado algún objeto, false si no

bool Seleccion( int x, int y, Escena * escena, ContextoVis & cv_dib )
{
   using namespace std ;
   assert( escena != nullptr );

   // COMPLETAR: práctica 5:
   // Visualizar escena en modo selección y leer el color del pixel en (x,y)
   // Se deben de dar estos pasos:

   //cout << "Seleccion( x == " << x << ", y == " << y << ", obj.raíz ==  " << objeto_raiz.leerNombre() << " )" << endl ;

   // 1. Crear (si es necesario) y activar el framebuffer object (fbo) de selección
   // .........
   if(fbo == nullptr)
      fbo = new Framebuffer(cv_dib.ventana_tam_x, cv_dib.ventana_tam_y);
   
   //fbo->activar(cv_dib.ventana_tam_x, cv_dib.ventana_tam_y);

   // 2. crear un 'ContextoVis' apropiado, en ese objeto:
   //    * activar modo selecion, desactivar iluminación, poner modo relleno
   //    * usar el mismo cauce que en 'cv_dib'
   //    * fijar el tamaño de la ventana igual que en 'cv_dib'
   //
   // ..........

   ContextoVis cv_seleccion = ContextoVis(cv_dib);

   cv_seleccion.modo_seleccion = true;
   cv_seleccion.iluminacion = false;
   cv_seleccion.modo_visu = ModosVisu::relleno;
   cv_seleccion.cauce = cv_dib.cauce;
   cv_seleccion.ventana_tam_x = cv_dib.ventana_tam_x;
   cv_seleccion.ventana_tam_y = cv_dib.ventana_tam_y;

   FijarColVertsIdent(*cv_seleccion.cauce,0);

   // 3. Activar fbo, cauce y viewport. Configurar cauce (modo solido relleno, sin ilum.
   //    ni texturas). Limpiar el FBO (color de fondo: 0)
   // .......

   cv_seleccion.cauce->activar();

   glViewport(0,0,cv_seleccion.ventana_tam_x,cv_seleccion.ventana_tam_y);
   FijarColVertsIdent(*cv_seleccion.cauce,0);
   fbo->activar(cv_seleccion.ventana_tam_x,cv_seleccion.ventana_tam_y);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   cv_seleccion.cauce->fijarEvalMIL(false);
   cv_seleccion.cauce->fijarEvalText(false);
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // limpieza FBO (especificado por el profesor)


   // 4. Activar la cámara (se debe leer de la escena con 'camaraActual')
   // ....
   escena->camaraActual()->activar(*cv_seleccion.cauce);

   // 5. Visualizar el objeto raiz actual (se debe leer de la escena con 'objetoActual()')
   //    la visualización se hace en modo selección.
   // ........
   escena->objetoActual()->visualizarGL(cv_seleccion);

   // 6. Leer el color del pixel (usar 'LeerIdentEnPixel')
   // (hay que hacerlo mientras está activado el framebuffer de selección)
   // .....
   int ident_pixel = LeerIdentEnPixel(x,y);

   // 7. Desactivar el framebuffer de selección
   // .....
   fbo->desactivar();

   // 8. Si el identificador del pixel es 0, imprimir mensaje y terminar (devolver 'false')
   // ....
   if(ident_pixel == 0)
   {
      cout << " No hay ningun objeto seleccionable en este pixel " << endl << flush;
      return false;
   }

   // 9. Buscar el objeto en el objeto_raiz (puede ser un grafo de escena)
   //    e informar del nombre del mismo (si no se encuentra, indicarlo)
   //   (usar 'buscarObjeto')
   // .....
   Objeto3D *buscado;
   Tupla3f centro;

   if(escena->objetoActual()->buscarObjeto(ident_pixel, MAT_Ident(), &buscado, centro))
   {
      EsferaEXP5 * pesf = dynamic_cast<EsferaEXP5 *> (buscado);

      if(pesf == nullptr)
      {
         escena->camaraActual()->mirarHacia(centro);
         cout << "Objeto: " << buscado->leerNombre() << " con identificador: " << buscado->leerIdentificador() << " encontrado " << endl;
         cout << "Se ha seleccionado la esfera número " << ((buscado->leerIdentificador()-1)%5)+1 << " de la fila numero " << (buscado->leerIdentificador()-1)/5 + 1 << endl;
      }
      else
      {
         // Si estaba seleccionada, es decir que estaba roja, la ponemos a color blanco
         if(pesf->seleccionada == true)
         {
            pesf->ponerColor({1.0,1.0,1.0});
            pesf->seleccionada = false;
         }
         else
         {
            pesf->ponerColor({1.0,0.5,0.5});
            pesf->seleccionada = true;
         }
            
      }
      
   }
   else
   { 
      cout << "No se ha encontrado el objeto" << endl;
      return false;
   }
   // al final devolvemos 'true', ya que hemos encontrado un objeto
   return true ;
}
