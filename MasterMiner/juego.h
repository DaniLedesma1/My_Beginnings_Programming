//Daniel Ledesma Ventura
//Nestor Marin Gomez

#ifndef JUEGO_H
#define JUEGO_H
#include "mina.h"
#include <conio.h>


typedef enum { ARRIBA, ABAJO, DCHA, IZDA, SALIR, NADA, TNT }tTecla;

typedef struct {
	tMina mina;
	int numGemas;
	int numMovs;
	int numDinamitas;
	int escala;  //1 -> escala 1:1   2 -> escala 1:3
	int modoEmpleo;  //1 -> usuario utiliza teclas  2 -> entrada teclas por fichero
	int estadoPartida;  //0->Gana, 1->Abandona, 2->Jugando
}tJuego;

tTecla leerTecla(std::istream & movimientos, tJuego &j);  
void cargar_Juego(tJuego& juego, int nivel); 
void dibujar(const tJuego& juego); 
void mostrarInfo(tJuego juego); 
void inicializarPartida(tJuego &j); 
void menuMovimientos(tJuego &j); 
void mostrarMov();
int elegirComoMover();
void jugar(tJuego& juego, std::istream & movimientos); 
void menuMov();
void caidaPiedras(tJuego & juego, int px, int py);
void leerMovimiento(tJuego & juego, tTecla& tecla, std::istream & movimientos); 
void realizarMovimiento(tJuego & juego, tTecla mov);
void caeDinamita(tJuego juego, int px, int py, int &dx);
void explotaDinamita(tJuego &juego, int px, int py);
void caePiedrasDinamita(tJuego &j, int px, int py);

#endif
