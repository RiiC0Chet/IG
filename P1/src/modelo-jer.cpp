#include "modelo-jer.h"

C::C()
{
    agregar( MAT_Traslacion( {5.0,5.0,5.0 }) );// Tra[0.1, 0.1]
    agregar(new Esfera(18,36));
}