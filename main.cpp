#include <cstring>
#include <fstream>
#include <iomanip>

using namespace std;

typedef char str30[31];
typedef char str20[21];
typedef char str10[11];
typedef unsigned short ushort;

const ushort MAX_ART = 10000;
const ushort MAX_CPRA = 100;
const ushort CANT_RUB = 15;
const ushort OFERTAS = 7;
const ushort TAM_LINEA = 104;

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

struct sRubArt {
    short codRub;
    int pos;
};

typedef sDescArt tvrIndArt[MAX_ART];
typedef sRub tvrRub[CANT_RUB];
typedef sCpra tvrLstCpra[MAX_CPRA];
typedef sRubArt tvrRubArt[MAX_ART];

void Abrir(fstream &Art, ifstream &IndArt, ifstream &Rub, ifstream &LstCpra) {
    Art.open("ArticulosModificado.txt");
    IndArt.open("IndDescripArt.txt");
    Rub.open("Rubros.txt");
    LstCpra.open("ListaCompras.txt");
}  // Abrir

void IntCmb(sRubArt &elem1, sRubArt &elem2) {
    sRubArt aux = elem1;
    elem1 = elem2;
    elem2 = aux;
}  // IntCmb

void OrdxBur(tvrRubArt &vrRubArt, ushort cantArt) {
    ushort k = 0;
    bool ordenado;

    do {
        ordenado = true;
        k++;

        for (ushort i = 0; i < cantArt - k; i++)
            if (vrRubArt[i].codRub > vrRubArt[i + 1].codRub) {
                ordenado = false;
                IntCmb(vrRubArt[i], vrRubArt[i + 1]);
            }

    } while (!ordenado);
}  // OrdxBur

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

void VolcarArchivos(fstream &Art, ifstream &IndArt, tvrIndArt &vrIndArt,
                    ifstream &Rub, tvrRub &vrRub, ifstream &LstCpra,
                    tvrLstCpra &vrLstCpra, tvrRubArt &vrRubArt, ushort &cantArt,
                    ushort &cantCpra) {
    sArt rArt;
    sDescArt rDescArt;
    sRub rRub;
    sCpra rCpra;

    while (LeerIndArt(IndArt, rDescArt) && cantArt < MAX_ART)
        vrIndArt[cantArt++] = rDescArt;

    while (LeerLstCpra(LstCpra, rCpra) && cantCpra < MAX_CPRA)
        vrLstCpra[cantCpra++] = rCpra;

    for (ushort i = 0; LeerRub(Rub, rRub) && i < CANT_RUB; i++)
        vrRub[i] = rRub;

    for (ushort i = 0; LeerArt(Art, rArt) && i < cantArt; i++) {
        vrRubArt[i].codRub = rArt.codRub;
        vrRubArt[i].pos = i;
    }

    OrdxBur(vrRubArt, cantArt);
}  // VolcarArchivos

int BusBinVec(tvrIndArt vrIndArt, str30 clave, ushort cantArt) {
    int ult = cantArt - 1;
    int prim = 0;
    int med;

    while (prim <= ult) {
        med = (prim + ult) / 2;

        if (strcmp(vrIndArt[med].descArt, clave) == 0)
            return med;
        else if (strcmp(vrIndArt[med].descArt, clave) < 0)
            prim = med + 1;
        else
            ult = med - 1;
    }

    return -1;
}  // BusBinVec

void ActLinea(fstream &Art, sArt &rArt, ushort posArt) {
    Art.clear();
    Art.seekp(TAM_LINEA * posArt);

    Art << setw(8) << rArt.codArt << " ";
    Art << setw(2) << rArt.codRub << " ";
    Art << setw(30) << rArt.descArt << " ";
    Art << setw(4) << rArt.stock << " ";
    Art << setw(9) << fixed << setprecision(2) << rArt.preUni << " ";
    Art << setw(10) << rArt.uniMed;

    for (ushort i = 0; i < OFERTAS; i++) {
        Art << " " << setw(1) << rArt.ofertas[i * 2] << " ";
        Art << setw(2) << rArt.ofertas[i * 2 + 1];
    }

    Art << "\n";
}

void ProcCompras(fstream &Art, tvrIndArt vrIndArt, tvrLstCpra &vrLstCpra,
                 ushort cantArt, ushort cantCpra) {
    sArt rArt;
    short posInd;
    ushort posArt;

    for (ushort i = 0; i < cantCpra; i++) {
        posInd = BusBinVec(vrIndArt, vrLstCpra[i].descArt, cantArt);

        if (posInd == -1 || !vrIndArt[posInd].estado) {
            vrLstCpra[i].cantReq = 0;
            continue;
        }

        posArt = vrIndArt[posInd].pos;
        Art.clear();
        Art.seekg(TAM_LINEA * posArt);
        LeerArt(Art, rArt);

        if (rArt.stock >= vrLstCpra[i].cantReq) {
            rArt.stock -= vrLstCpra[i].cantReq;

        } else {
            vrLstCpra[i].cantReq = rArt.stock;
            rArt.stock = 0;
        }

        ActLinea(Art, rArt, posArt);
    }
}

void Cerrar(fstream &Art, ifstream &IndArt, ifstream &Rub, ifstream &LstCpra) {
    Art.close();
    IndArt.close();
    Rub.close();
    LstCpra.close();
}  // Cerrar

int main() {
    tvrIndArt vrIndArt;
    tvrRub vrRub;
    tvrLstCpra vrLstCpra;
    tvrRubArt vrRubArt;

    fstream Art;
    ifstream IndArt;
    ifstream Rub;
    ifstream LstCpra;

    ushort cantArt = 0;
    ushort cantCpra = 0;

    Abrir(Art, IndArt, Rub, LstCpra);
    VolcarArchivos(Art, IndArt, vrIndArt, Rub, vrRub, LstCpra, vrLstCpra,
                   vrRubArt, cantArt, cantCpra);
    ProcCompras(Art, vrIndArt, vrLstCpra, cantArt, cantCpra);
    Cerrar(Art, IndArt, Rub, LstCpra);
    return 0;
}