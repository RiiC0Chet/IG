#include "modelo-jer.h"

C::C(float alpha)
{
    agregar( MAT_Traslacion( { 1.0, 1.0 , 1.0 }) );
    int  indicie_1 = agregar(MAT_Rotacion( alpha, { 0.0, 1.0, 0.0 } ));
    agregar(new Esfera(18,36));

    // Almacenamos la matriz vvariable con el puntero como variable de instancia pm_tras
    pm_tras = leerPtrMatriz(indicie_1);
}
unsigned C::leerNumParametros() const 
{
    return 1;
}

void C::actualizarEstadoParametro( const unsigned iParam, const float t_sec )
{
    assert(iParam < leerNumParametros() && iParam >=0);

    switch(iParam)
    {
        case 0:
            float alpha = ( (90+360)/2 ) + ( (360-90)/2 ) * sin(2*M_PI*t_sec);
            *pm_tras = MAT_Rotacion( alpha, { 0.0, 1.0, 0.0 } ) ;
        break;
    }

}



PlataformaInferior::PlataformaInferior()
{
    agregar(MAT_Escalado(0.85,0.25,0.85));
    agregar(MAT_Traslacion({0.5,0.5,0.5}));
    agregar(new Cubo());
}

PilarGrua::PilarGrua()
{
    agregar(new PlataformaInferior());
    agregar(MAT_Escalado(0.5,1,0.5));
    agregar(MAT_Traslacion({0.87,0.35,0.87}));
    agregar(new Cilindro(10,40));
    agregar(MAT_Traslacion({0.0,1,0.0}));
    agregar(new Cilindro(10,40));
    agregar(MAT_Traslacion({0.0,1,0.0}));
    agregar(new Cilindro(10,40));
    agregar(MAT_Traslacion({0.0,1,0.0}));
    agregar(new Cilindro(10,40));

}
