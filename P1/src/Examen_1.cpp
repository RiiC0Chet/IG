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


//----------------Clase Varita--------------------

Varita::Varita()
{
    agregar(MAT_Escalado(1.0,10.0,1.0));
    agregar(new Cubo());
}

//------------------------------------------------