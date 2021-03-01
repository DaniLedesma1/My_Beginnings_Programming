//Daniel Ledesma Ventura
//Nestor Marin Gomez

#include "mina.h"
#include <Windows.h>
#include <iomanip>
using namespace std;

const int ESCALA = 3;


void cargar_Mina(ifstream& fichero, tMina& mina) {

	char aux, basura;
	fichero >> mina.nfil >> mina.ncol;
	fichero.get(basura); //Para no tomar el salto de linea
	for (int i = 0; i < mina.nfil; i++) {
		for (int j = 0; j < mina.ncol; j++) {
			fichero.get(aux);  //Necesario el .get para leer espacios en blanco
			mina.p[i][j] = chartoCasilla(aux);
			if (mina.p[i][j] == MINERO) {
				mina.filMinero = i;
				mina.colMinero = j;
			}
		}
		fichero.get(basura); //Ignoramos salto de linea
	}
}

void dibujar1_1(const tMina& mina) {
	system("cls");
	for (int i = 0; i < mina.nfil; i++) {
		for (int j = 0; j < mina.ncol; j++) {
			pintarCasilla(mina.p[i][j]);
		}
		cout << endl;
	}
	cout << endl;
}


void dibujar1_3(const tMina& mina) {

	for (int i = 0; i < mina.nfil; ++i) {
		for (int l = 0; l < ESCALA; ++l) {
			for (int j = 0; j < mina.ncol; ++j) {
				for (int k = 0; k < ESCALA; ++k) {
					pintarCasilla(mina.p[i][j]);
				}
			}
			cout << endl;
		}
	}
	cout << endl;
}

tCasilla chartoCasilla(char aux) {
	if (aux == 'G') {
		return GEMA;
	}
	else if (aux == 'M') {
		return MURO;
	}
	else if (aux == 'T') {
		return TIERRA;
	}
	else if (aux == 'P') {
		return PIEDRA;
	}
	else if (aux == 'J') {
		return MINERO;
	}
	else if (aux == 'S') {
		return SALIDA;
	}
	else {
		return LIBRE;
	}
}



void pintarCasilla(tCasilla c) {
	if (c == PIEDRA) {
		colorFondo(4);
		cout << '@';
		colorFondo(0);
	}
	else if (c == MURO) {
		colorFondo(4);
		cout << 'X';
		colorFondo(0);
	}
	else if (c == TIERRA) {
		colorFondo(4);
		cout << '.';
		colorFondo(0);
	}
	else if (c == MINERO) {
		colorFondo(8);
		cout << 'M';
		colorFondo(0);
	}
	else if (c == DINAMITA) {
		colorFondo(4);
		cout << 'D';
		colorFondo(0);
	}
	else if (c == GEMA) {
		colorFondo(2);
		cout << 'G';
		colorFondo(0);
	}
	else if (c == SALIDA) {
		colorFondo(1);
		cout << 'S';
		colorFondo(0);
	}
	else {
		colorFondo(4);
		cout << ' ';
		colorFondo(0);
	}
}

void colorFondo(int color) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, 15 | (color << 4));
}