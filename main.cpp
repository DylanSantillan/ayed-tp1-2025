#include <fstream>

using namespace std;

typedef char str30[31];
typedef char str20[21];
typedef char str10[11];
typedef unsigned short ushort;

const ushort MAX_ART = 10000;
const ushort MAX_CPRA = 100;
const ushort CANT_RUB = 15;
const ushort OFERTAS = 7;

struct sArt {
    int codArt;
    short codRub;
    str30 descArt;
    ushort stock;
    float preUni;
    str10 uniMed;
    short ofertas[OFERTAS * 2];
};

struct sDescArt {
    str30 descArt;
    int pos;
    bool estado;
};

struct sRub {
    short codRub;
    str20 descRub;
};

struct sCpra {
    str30 descArt;
    short cantReq;
};

typedef sArt tvrArt[MAX_ART];
typedef sDescArt tvrIndArt[MAX_ART];
typedef sRub tvrRub[CANT_RUB];
typedef sCpra tvrLstCpra[MAX_CPRA];

void Abrir(fstream &Art, ifstream &IndArt, ifstream &Rub, ifstream &LstCpra) {
    Art.open("Articulos.txt");
    IndArt.open("IndDescripArt.txt");
    Rub.open("Rubros.txt");
    LstCpra.open("ListaCompras.txt");
}  // Abrir

bool LeerArt(fstream &Art, sArt &rArt) {
    Art >> rArt.codArt;
    Art >> rArt.codRub;
    Art.ignore();
    Art.get(rArt.descArt, 31);
    Art >> rArt.stock;
    Art >> rArt.preUni;
    Art.ignore();
    Art.get(rArt.uniMed, 11);

    for (ushort i = 0; i < OFERTAS * 2; i++)
        Art >> rArt.ofertas[i];

    Art.ignore();

    return Art.good();
}  // LeerArt

bool LeerIndArt(ifstream &IndArt, sDescArt &rDescArt) {
    IndArt.get(rDescArt.descArt, 31);
    IndArt >> rDescArt.pos;
    IndArt >> rDescArt.estado;
    IndArt.ignore();

    return IndArt.good();
}  // LeerIndArt

bool LeerRub(ifstream &Rub, sRub &rRub) {
    Rub >> rRub.codRub;
    Rub.ignore();
    Rub.get(rRub.descRub, 21);
    Rub.ignore();

    return Rub.good();
}  // LeerRub

bool LeerLstCpra(ifstream &LstCpra, sCpra &rCpra) {
    LstCpra.get(rCpra.descArt, 31);
    LstCpra >> rCpra.cantReq;
    LstCpra.ignore();

    return LstCpra.good();
}  // LeerLstCpra

void VolcarArchivos(fstream &Art, tvrArt &vrArt, ifstream &IndArt,
                    tvrIndArt &vrIndArt, ifstream &Rub, tvrRub &vrRub,
                    ifstream &LstCpra, tvrLstCpra &vrLstCpra, ushort &cantArt,
                    ushort &cantCpra) {
    sArt rArt;
    sDescArt rDescArt;
    sRub rRub;
    sCpra rCpra;

    while (LeerArt(Art, rArt) && cantArt < MAX_ART)
        vrArt[cantArt++] = rArt;

    for (ushort i = 0; LeerIndArt(IndArt, rDescArt) && i < cantArt; i++)
        vrIndArt[i] = rDescArt;

    for (ushort i = 0; LeerRub(Rub, rRub) && i < CANT_RUB; i++)
        vrRub[i] = rRub;

    while (LeerLstCpra(LstCpra, rCpra) && cantCpra < MAX_CPRA)
        vrLstCpra[cantCpra++] = rCpra;
}  // VolcarArchivos

void Cerrar(fstream &Art, ifstream &IndArt, ifstream &Rub, ifstream &LstCpra) {
    Art.close();
    IndArt.close();
    Rub.close();
    LstCpra.close();
}  // Cerrar

int main() {
    tvrArt vrArt;
    tvrIndArt vrIndArt;
    tvrRub vrRub;
    tvrLstCpra vrLstCpra;

    fstream Art;
    ifstream IndArt;
    ifstream Rub;
    ifstream LstCpra;

    ushort cantArt = 0;
    ushort cantCpra = 0;

    Abrir(Art, IndArt, Rub, LstCpra);
    VolcarArchivos(Art, vrArt, IndArt, vrIndArt, Rub, vrRub, LstCpra, vrLstCpra,
                   cantArt, cantCpra);
    Cerrar(Art, IndArt, Rub, LstCpra);

    return 0;
}