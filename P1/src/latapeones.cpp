#include "latapeones.h"
#include "malla-revol.h"

Lata::Lata()
{

    MallaRevolPLY * lata = new MallaRevolPLY("lata-pcue.ply",15);
    
    Textura * cocacola = new Textura ("lata-coke.jpg");

    Material * material_luces = new Material(0.4,0.4,12,140);

    //ambiental 
    //difuso 
    //pseudo es el que refleja

    //0.8,0.8,12,140
    Material * mat = new Material(cocacola,0.8,0.8,12,140);

    agregar(material_luces);
    agregar(new MallaRevolPLY("lata-psup.ply",15));
    agregar(new MallaRevolPLY("lata-pinf.ply",15));
    agregar(mat);
    agregar(lata);
}

PeonMadera::PeonMadera()
{
    // Material difuso-especular
    Material * mat = new Material(new TexturaXY ("text-madera.jpg"),0.25, 0.75, 1.0, 50.0);

    agregar(MAT_Escalado(0.3,0.3,0.3));
    agregar(mat);
    agregar(new MallaRevolPLY("peon.ply",15));
}

PeonBlanco::PeonBlanco()
{
    // Material puramente difuso
    Material * material_luces = new Material(0.1, 1.0, 0.0, 1.0);
    agregar(MAT_Escalado(0.3,0.3,0.3));
    agregar(material_luces);
    agregar(new MallaRevolPLY("peon.ply",15));
}

PeonNegro::PeonNegro()
{
    // Material sin apenas reflectividad difusa
    Material * material_luces = new Material(0.0,0.3,1.0,50.0);
    agregar(MAT_Escalado(0.3,0.3,0.3));
    agregar(material_luces);
    agregar(new MallaRevolPLY("peon.ply",15));
}

LataPeones::LataPeones()
{
    agregar(new Lata());

    agregar(MAT_Traslacion({0.0,0.0,1.0}));
    agregar(new PeonMadera());
    agregar(MAT_Traslacion({0.8,0.0,0.0}));
    agregar(new PeonBlanco());
    agregar(MAT_Traslacion({0.8,0.0,0.0}));
    agregar(new PeonNegro());
}