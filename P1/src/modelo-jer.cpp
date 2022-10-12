#include "modelo-jer.h"

C::C(float traslacion)
{
    int  indicie_1 = agregar( MAT_Traslacion( { traslacion, traslacion, traslacion }) );// Tra[0.1, 0.1]
    agregar(new Esfera(18,36));

    // Almacenamos la matriz vvariable con el puntero como variable de instancia pm_tras
    pm_tras = leerPtrMatriz(indicie_1);
}
unsigned C::leerNumParametros() const 
{
    return 1;
}