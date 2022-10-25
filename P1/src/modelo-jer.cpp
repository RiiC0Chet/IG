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
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



PlataformaCuadrada::PlataformaCuadrada()
{
    agregar(MAT_Escalado(0.85,0.25,0.85));
    agregar(MAT_Traslacion({0.5,0.5,0.5}));
    agregar(new Cubo());
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
PilarGrua::PilarGrua()
{
    agregar(new PlataformaCuadrada());
    agregar(MAT_Escalado(0.5,1,0.5));
    agregar(MAT_Traslacion({0.87,0.35,0.87}));
    agregar(new Cilindro(10,40));
    agregar(MAT_Traslacion({0.0,1,0.0}));
    agregar(new Cilindro(10,40));
    agregar(MAT_Traslacion({0.0,1,0.0}));
    agregar(new Cilindro(10,40));
    agregar(MAT_Traslacion({0.0,1,0.0}));
    agregar(new Cilindro(10,40));
    this->ponerColor({0.9,0.1,0.5});

}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
PlataformaDesplazante::PlataformaDesplazante()
{
    agregar(MAT_Escalado(0.65,0.75,0.65));
    agregar(new PlataformaCuadrada());

    this->ponerColor({0.1,1.0,0.4});

    // Matriz que vamos a modificar como grado de libertad 
    int indice_1 = agregar(MAT_Escalado(0.1,1.0,0.1));
    agregar(MAT_Traslacion({4.5,-1.05,4.5}));
    agregar(new Cilindro(10,40));
    
    

    // Almacenamos la matriz vvariable con el puntero como variable de instancia pm_tras
    pm_tras = leerPtrMatriz(indice_1);
}

unsigned PlataformaDesplazante::leerNumParametros() const 
{
    return 1;
}

void PlataformaDesplazante::actualizarEstadoParametro( const unsigned iParam, const float t_sec )
{
    assert(iParam < leerNumParametros() && iParam >=0);

    switch(iParam)
    {
        case 0:
            *pm_tras = MAT_Escalado(0.1,1.0+(t_sec/10),0.1) ;
        break;
    }

}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
PlataformaArriba::PlataformaArriba()
{
    agregar(MAT_Traslacion({0.0,-0.2,0.0}));
    agregar(MAT_Escalado(0.6,0.6,0.6));
    agregar(new Cubo());
    agregar(MAT_Traslacion({0.0,0,2.0}));
    agregar(new Cubo());
    agregar(MAT_Traslacion({0.0,0,2.0}));
    agregar(new Cubo());
    agregar(MAT_Traslacion({0.0,0,2.0}));
    agregar(new Cubo());
    agregar(MAT_Traslacion({0.0,0,2.0}));
    agregar(new Cubo());
    agregar(MAT_Traslacion({0.0,0,2.0}));
    agregar(new Cubo());
    agregar(MAT_Traslacion({-0.25,-1.3,-4.5}));

    this->ponerColor({0.1,0.2,0.8});

    int indice_1 = agregar(MAT_Traslacion({0.0,0.0,0.0}));
    plataforma_desplazante = new PlataformaDesplazante();
    agregar(plataforma_desplazante);

    pm_tras = leerPtrMatriz(indice_1);
}

unsigned PlataformaArriba::leerNumParametros() const 
{
    return 2;
}

void PlataformaArriba::actualizarEstadoParametro( const unsigned iParam, const float t_sec )
{
    assert(iParam < leerNumParametros() && iParam >=0);

    switch(iParam)
    {
        case 0:
            plataforma_desplazante->actualizarEstadoParametro(iParam,t_sec);
        break;
        case 1:
            float desplazamiento = -1*4*sin(t_sec/1.8);
            *pm_tras = MAT_Traslacion({0.0,0.0,desplazamiento});
        break;
    }

}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Grua::Grua(float alpha)
{
    agregar(new PilarGrua());
    agregar(MAT_Traslacion({0.45,5.0,0.45}));
    int  indicie_1 = agregar(MAT_Rotacion( alpha, { 0.0, 1.0, 0.0 } ));
    plataforma_arriba = new PlataformaArriba();
    agregar(plataforma_arriba);

    pm_rot = leerPtrMatriz(indicie_1);
}

unsigned Grua::leerNumParametros() const 
{
    return 3;
}

void Grua::actualizarEstadoParametro( const unsigned iParam, const float t_sec )
{
    assert(iParam < leerNumParametros() && iParam >=0);

    switch(iParam)
    {
        case 0:
            plataforma_arriba->actualizarEstadoParametro(iParam,t_sec);
        break;
        case 1:
            plataforma_arriba->actualizarEstadoParametro(iParam,t_sec);
        break;
        case 2:
            float alpha = ( (0+360)/2 ) + ( (360-0)/2 ) * sin((M_PI*t_sec)/6);
            *pm_rot = MAT_Rotacion( 360*t_sec/10, { 0.0, 1.0, 0.0 } ) ;
        break;
    }

}