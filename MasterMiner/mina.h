//Daniel Ledesma Ventura
//Nestor Marin Gomez

#ifndef MINA_H
#define MINA_H

#include <string>
#include <iostream>
#include <fstream>
using namespace std;


const int MAX = 50;


typedef enum { LIBRE, TIERRA, GEMA, PIEDRA, MURO, SALIDA, MINERO, DINAMITA }tCasilla;

typedef tCasilla tPlano[MAX][MAX];


typedef struct{
	tPlano p;
	int nfil;
	int ncol;
	int filMinero;
	int colMinero;

}tMina;


void cargar_Mina(ifstream& fichero, tMina& mina);
void dibujar1_1(const tMina& mina);
void dibujar1_3(const tMina& mina);
void pintarCasilla(tCasilla c);
tCasilla chartoCasilla(char aux);
void colorFondo(int color);

#endif