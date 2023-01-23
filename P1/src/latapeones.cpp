// Nombre: Jose Luis Apellidos: Rico Ramos Titulación: GIADE
// email: jlricoramos@correo.ugr.es DNI o pasaporte: 20080849D
#include "latapeones.h"
#include "malla-revol.h"

Lata::Lata(std::string archivo)
{

    MallaRevolPLY * lata = new MallaRevolPLY("lata-pcue.ply",15);
    
    Textura * cocacola = new Textura (archivo);

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
    int ident_peon_madera = 2;
    // Material difuso-especular
    Material * mat = new Material(new TexturaXY ("text-madera.jpg"),0.25, 0.75, 1.0, 50.0);

    agregar(MAT_Escalado(0.3,0.3,0.3));
    agregar(mat);
    agregar(new MallaRevolPLY("peon.ply",15));

    ponerNombre("Peón de madera");
    ponerIdentificador(ident_peon_madera);
}

PeonBlanco::PeonBlanco()
{
    int ident_peon_blanco = 3;
    // Material puramente difuso
    Material * material_luces = new Material(0.1, 1.0, 0.0, 1.0);
    agregar(MAT_Escalado(0.3,0.3,0.3));
    agregar(material_luces);
    agregar(new MallaRevolPLY("peon.ply",15));

    ponerNombre("Peón blanco");
    ponerIdentificador(ident_peon_blanco);
}

PeonNegro::PeonNegro()
{
    int ident_peon_negro = 4;
    // Material sin apenas reflectividad difusa
    Material * material_luces = new Material(0.0,0.3,1.0,50.0);
    agregar(MAT_Escalado(0.3,0.3,0.3));
    agregar(material_luces);
    agregar(new MallaRevolPLY("peon.ply",15));

    ponerNombre("Peón negro");
    ponerIdentificador(ident_peon_negro);
}

LataPeones::LataPeones()
{
    int ident_cola = 1;
    
    
    

    NodoGrafoEscena * cola_n = new NodoGrafoEscena();

    cola_n->agregar(new Lata("lata-coke.jpg"));
    cola_n->ponerNombre("Lata de Coca-Cola");
    cola_n->ponerIdentificador(ident_cola);

    agregar(cola_n);
    

    agregar(MAT_Traslacion({0.0,0.0,1.0}));
    agregar(new PeonMadera());
    agregar(MAT_Traslacion({0.8,0.0,0.0}));
    agregar(new PeonBlanco());
    agregar(MAT_Traslacion({0.8,0.0,0.0}));
    agregar(new PeonNegro());
}

VariasLatasPeones::VariasLatasPeones()
{
    int ident_pepsi = 5;
    int ident_ugr = 6;

    NodoGrafoEscena * pepsi_n = new NodoGrafoEscena();
    NodoGrafoEscena * ugr_n = new NodoGrafoEscena();

    Lata * coca = new Lata("lata-coke.jpg");
    Lata * pepsi = new Lata("lata-pepsi.jpg");
    Lata * ugr = new Lata("window-icon.jpg");

    

    pepsi_n->agregar(MAT_Traslacion({1.0,0.0,0.0}));
    pepsi_n->agregar(pepsi);
    pepsi_n->ponerNombre("Lata de Pepsi");
    pepsi_n->ponerIdentificador(ident_pepsi);

    ugr_n->agregar(MAT_Traslacion({2.0,0.0,0.0}));
    ugr_n->agregar(ugr);
    ugr_n->ponerNombre("Lata de la UGR");
    ugr_n->ponerIdentificador(ident_ugr);

    agregar(pepsi_n);
    agregar(ugr_n);

    agregar(new LataPeones());
}
