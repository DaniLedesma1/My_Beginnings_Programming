//Daniel Ledesma Ventura
//Nestor Marin Gomez

#include "juego.h"
#include <Windows.h>
using namespace std;


tTecla leerTecla(std::istream &m, tJuego &j) {
	if (j.modoEmpleo == 1) {
		int dir;
		cin.sync();
		dir = _getch(); // dir: tipo int
		if (dir == 0xe0) {
			dir = _getch();
			if (dir == 72) {
				return ARRIBA;
			}
			else if (dir == 80) {
				return ABAJO;
			}
			else if (dir == 77) {
				return DCHA;
			}
			else return IZDA;
		}
		else if (dir == 27) {
			j.estadoPartida = 1;
			return SALIR;
		}
		else if (dir == 100) {
			return TNT;
		}
		else return NADA;
	}
	else {
		char tecla;
		m >> tecla;
		if (tecla == 'D') {
			return TNT;
		}
		else if (tecla == 'M') {
			return DCHA;
		}
		else if (tecla == 'N') {
			return IZDA;
		}
		else if (tecla == 'A') {
			return ARRIBA;
		}
		else return ABAJO;
	}
}


void cargar_Juego(tJuego& juego, int nivel) {
	
	string fichero;
	ifstream archivo;

	fichero = to_string(nivel) + ".txt";
	archivo.open(fichero);

	if (!archivo.is_open()) {
		cout << "ERROR AL CARGAR EL ARCHIVO: " << fichero << endl;
		
	}
	else {
		menuMovimientos(juego); //Necesitamos guardar en juego.modoEmpleo
								//como se introduciran los movimientos
		cargar_Mina(archivo, juego.mina);
		inicializarPartida(juego);
	}
}



void dibujar(const tJuego& juego) {
	if (juego.escala == 1) {
		dibujar1_1(juego.mina);
		mostrarInfo(juego);
	}
	else {
		dibujar1_3(juego.mina);
		mostrarInfo(juego);
	}
}

void mostrarInfo(tJuego juego) {
	cout << "Numero de gemas recogidas: " << juego.numGemas << endl;
	cout << "Numero de movimientos realizados: " << juego.numMovs << endl;
	cout << "Numero de dinamitas utilizadas: " << juego.numDinamitas << endl << endl;
}

void inicializarPartida(tJuego &j) {
	j.numGemas = 0;
	j.numDinamitas = 0;
	j.numMovs = 0;
	j.estadoPartida = 2;
}


void menuMovimientos(tJuego &j) {
	int op;
	mostrarMov();
	op = elegirComoMover();
	j.modoEmpleo = op;
}

void mostrarMov() {
	cout << endl << "1. Introducir movimientos por teclado" << endl;
	cout << "2. Introducir movimientos por fichero" << endl;
}

int elegirComoMover() {
	int op;
	cout << "Elija una opcion: ";
	cin >> op;
	while (cin.fail() || op > 2 || op < 1) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "Error: opcion escogida no valida. " << endl;
		cout << "Elija una opcion: ";
		cin >> op;
	}
	cin.sync();
	return op;
}


void menuMov() {
	cout << "Opciones de Movimiento disponibles:" << endl;
	cout << "  -   DERECHA:    Flecha derecha " << endl;
	cout << "  -   IZQUIERDA:  Flecha izquierda " << endl;
	cout << "  -   ARRIBA:     Flecha arriba " << endl;
	cout << "  -   ABAJO:      Flecha abajo " << endl;
	cout << "  -   DINAMITA:   Pulse d  " << endl;
	cout << "  -   SALIR:	  Escape " << endl;
}

void jugar(tJuego& juego, std::istream & movimientos) {
	tTecla tecla;
	while (juego.estadoPartida == 2) {
		if (juego.modoEmpleo == 1) {
			system("cls");
			dibujar(juego);
			leerMovimiento(juego, tecla, std::cin);
			if (!(juego.estadoPartida == 1)) {
				realizarMovimiento(juego, tecla);
				dibujar(juego);
			}
			
		}
		else {
			system("cls");
			dibujar(juego);
			leerMovimiento(juego, tecla, movimientos);
			if (!(juego.estadoPartida == 1)) {
				realizarMovimiento(juego, tecla);
				dibujar(juego);
				Sleep(1000);
			}
		}
	}
}

void leerMovimiento(tJuego & juego, tTecla& tecla, std::istream & movimientos) {
	if (juego.modoEmpleo == 2) {
		tecla = leerTecla(movimientos, juego);
	}
	else {
		menuMov();
		tecla = leerTecla(std::cin, juego);
		while (tecla == NADA) {
			tecla = leerTecla(std::cin, juego);
		}
	}

}


void caidaPiedras(tJuego & j, int px, int py) {

	int p2 = px;
	int pxaux = px + 1;//Primera iteracion tenemos caida asegurada
	int cont = 1;
	int despl;
	while (p2 >= 0 && (j.mina.p[p2][py] == GEMA || j.mina.p[p2][py] == PIEDRA)) {
		
		pxaux--;
		despl = pxaux;
		
		while ((despl + cont < j.mina.nfil) && (j.mina.p[despl + cont][py] == LIBRE)) {
			if (j.mina.p[despl][py] == GEMA) {
				j.mina.p[despl][py] = LIBRE;
				j.mina.p[despl + cont][py] = GEMA;
			}
			else if (j.mina.p[despl][py] == PIEDRA) {
				j.mina.p[despl][py] = LIBRE;
				j.mina.p[despl + cont][py] = PIEDRA;
			}
			despl++;
		}
		p2--;
	}

}

void realizarMovimiento(tJuego & j, tTecla mov) {
	int x = j.mina.filMinero;
	int y = j.mina.colMinero;
	if (mov == ARRIBA) {
		if (( x - 1) >= 0) {
			if (j.mina.p[x-1][y] == GEMA) {
				j.numGemas++;
				j.numMovs++;
				j.mina.filMinero = x - 1;
				j.mina.p[x - 1][y] = MINERO;
				j.mina.p[x][y] = LIBRE;
			}
			else if (j.mina.p[x - 1][y] == TIERRA || j.mina.p[x - 1][y] == LIBRE) {
				j.numMovs++;
				j.mina.filMinero = x - 1;
				j.mina.p[x - 1][y] = MINERO;
				j.mina.p[x][y] = LIBRE;
			}
			else if (j.mina.p[x - 1][y] == SALIDA) {
				j.numMovs++;
				j.estadoPartida = 0;
				j.mina.p[x][y] = LIBRE;
				system("cls");
			}
		}
	}
	else if (mov == ABAJO) {
		if ((x + 1) < j.mina.nfil) {
			if (j.mina.p[x + 1][y] == GEMA) {
				j.numGemas++;
				j.numMovs++;
				j.mina.filMinero = x + 1;
				j.mina.p[x + 1][y] = MINERO;
				j.mina.p[x][y] = LIBRE;
				if ((x - 1) >= 0) {
					if (j.mina.p[x - 1][y] == GEMA || j.mina.p[x - 1][y] == PIEDRA) {
						caidaPiedras(j, x-1 , y);
					}
				}
				
			}
			else if (j.mina.p[x + 1][y] == TIERRA || j.mina.p[x + 1][y] == LIBRE) {
				j.numMovs++;
				j.mina.filMinero = x + 1;
				j.mina.p[x + 1][y] = MINERO;
				j.mina.p[x][y] = LIBRE;
				if ((x - 1) >= 0) {
					if (j.mina.p[x - 1][y] == GEMA || j.mina.p[x - 1][y] == PIEDRA) {
						caidaPiedras(j, x-1, y);
					}
				}
			}
			else if (j.mina.p[x + 1][y] == SALIDA) {
				j.numMovs++;
				j.estadoPartida = 0;
				j.mina.p[x][y] = LIBRE;
				system("cls");
			}
		}
	}
	else if (mov == DCHA) {
		if ((y + 1) < j.mina.ncol) {
			if (j.mina.p[x ][y+ 1] == GEMA) {
				j.numGemas++;
				j.numMovs++;
				j.mina.colMinero = y + 1;
				j.mina.p[x][y + 1] = MINERO;
				j.mina.p[x][y] = LIBRE;
				if ((x - 1) >= 0) {
					if (j.mina.p[x - 1][y] == GEMA || j.mina.p[x - 1][y] == PIEDRA) {
						caidaPiedras(j, x-1, y);
					}
				}
			}
			else if (j.mina.p[x][y + 1] == TIERRA || j.mina.p[x][y + 1] == LIBRE) {
				j.numMovs++;
				j.mina.colMinero = y + 1;
				j.mina.p[x][y + 1] = MINERO;
				j.mina.p[x][y] = LIBRE;
				if ((x - 1) >= 0) {
					if (j.mina.p[x - 1][y] == GEMA || j.mina.p[x - 1][y] == PIEDRA) {
						caidaPiedras(j, x-1, y);
					}
				}
			}
			else if (j.mina.p[x][y + 1] == SALIDA) {
				j.numMovs++;
				j.estadoPartida = 0;
				j.mina.p[x][y] = LIBRE;
				system("cls");
			}
			else if (j.mina.p[x][y + 1] == PIEDRA) {
				if ((y + 2) < j.mina.ncol) {
					if (j.mina.p[x][y + 2] == LIBRE) {
						j.numMovs++;
						j.mina.colMinero = y + 1;
						j.mina.p[x][y + 1] = MINERO;
						j.mina.p[x][y + 2] = PIEDRA;
						j.mina.p[x][y] = LIBRE;
						if ((x - 1) >= 0) {
							if (j.mina.p[x - 1][y] == GEMA || j.mina.p[x - 1][y] == PIEDRA) {
								caidaPiedras(j, x-1, y);
							}
						}
						if ((x + 1) < j.mina.nfil) {
							if (j.mina.p[x + 1][y+2] == LIBRE) {
								caidaPiedras(j, x, y+2);
							}
						}
					}
				}
			}
		}
	}
	else if (mov == IZDA) {
		if ((y - 1) >= 0) {
			if (j.mina.p[x][y - 1] == GEMA) {
				j.numGemas++;
				j.numMovs++;
				j.mina.colMinero = y - 1;
				j.mina.p[x][y - 1] = MINERO;
				j.mina.p[x][y] = LIBRE;
				if ((x - 1) >= 0) {
					if (j.mina.p[x - 1][y] == GEMA || j.mina.p[x - 1][y] == PIEDRA) {
						caidaPiedras(j, x-1, y);
					}
				}
			}
			else if (j.mina.p[x][y - 1] == TIERRA || j.mina.p[x][y - 1] == LIBRE) {
				j.numMovs++;
				j.mina.colMinero = y - 1;
				j.mina.p[x][y - 1] = MINERO;
				j.mina.p[x][y] = LIBRE;
				if ((x - 1) >= 0) {
					if (j.mina.p[x - 1][y] == GEMA || j.mina.p[x - 1][y] == PIEDRA) {
						caidaPiedras(j, x-1, y);
					}
				}
			}
			else if (j.mina.p[x][y - 1] == SALIDA) {
				j.numMovs++;
				j.estadoPartida = 0;
				j.mina.p[x][y] = LIBRE;
				system("cls");
			}
			else if (j.mina.p[x][y - 1] == PIEDRA) {
				if ((y - 2) >= 0) {
					if (j.mina.p[x][y - 2] == LIBRE) {
						j.numMovs++;
						j.mina.colMinero = y - 1;
						j.mina.p[x][y - 1] = MINERO;
						j.mina.p[x][y - 2] = PIEDRA;
						j.mina.p[x][y] = LIBRE;
						if ((x - 1) >= 0) {
							if (j.mina.p[x - 1][y] == GEMA || j.mina.p[x - 1][y] == PIEDRA) {
								caidaPiedras(j, x-1, y);
							}
						}
						if ((x + 1) < j.mina.nfil) {
							if (j.mina.p[x + 1][y - 2] == LIBRE) {
								caidaPiedras(j, x, y-2);
							}
						}
					}
				}
			}
		}
	}
	else if (mov == TNT) {
		int dx = 0;
		j.numDinamitas++;
		caeDinamita(j, x, y, dx); //dx retorna como la fila en la que se produce el epicentro de la explosion
		explotaDinamita(j, dx, y);
		caePiedrasDinamita(j, dx, y);
	}
}


void caeDinamita(tJuego j, int px, int py, int &dx) {
	
	int cont = 1;
	if (!(j.mina.p[px + 1][py] == LIBRE)) {
		j.mina.p[px][py] = DINAMITA;
		dx = px;
		dibujar(j);
		Sleep(1000);
	}
	else {
		while (j.mina.p[px + cont + 1][py] == LIBRE) {
			cont++;
		}
		dx = px + cont;
		j.mina.p[dx][py] = DINAMITA;
		dibujar(j);
		Sleep(1000);
	}
	
}

void explotaDinamita(tJuego &j, int px, int py) {
	if (j.mina.p[px][py] == MINERO) { //Posicion central
		j.estadoPartida = 1;
		j.mina.p[px][py] = LIBRE;
	}
	else {                            //Posicion central
		j.mina.p[px][py] = LIBRE;
	}
	if ((px - 1) >= 0) {              //Posicion arriba
		if (j.mina.p[px - 1][py] == MINERO) {
			j.estadoPartida = 1;
			j.mina.p[px - 1][py] = LIBRE;
		}
		else
			j.mina.p[px - 1][py] = LIBRE;
	}
	
	if (((py + 1) < j.mina.ncol) && !(j.mina.p[px][py + 1] == SALIDA)) {      //Posicion derecha
		j.mina.p[px][py + 1] = LIBRE;
	}
	if (((py - 1) >= 0) && !(j.mina.p[px][py - 1] == SALIDA)) {               //Posicion izquierda
		j.mina.p[px][py - 1] = LIBRE;
	}
	if (((px + 1) < j.mina.nfil) && !(j.mina.p[px + 1][py] == SALIDA)) {      //Posicion abajo
		j.mina.p[px + 1][py] = LIBRE;
	}
	if (((px + 1) < j.mina.nfil) && ((py + 1) < j.mina.ncol) && !(j.mina.p[px + 1][py + 1] == SALIDA)) {    //Posicion abajo-derecha
		j.mina.p[px + 1][py + 1] = LIBRE;
	}
	if (((px + 1) < j.mina.nfil) && ((py - 1) >= 0) && !(j.mina.p[px + 1][py - 1] == SALIDA)) {    //Posicion abajo-izquierda
		j.mina.p[px + 1][py - 1] = LIBRE;
	}
	if (((px - 1) >= 0) && ((py + 1) < j.mina.ncol) && !(j.mina.p[px - 1][py + 1] == SALIDA)) {    //Posicion arriba-derecha
		j.mina.p[px - 1][py + 1] = LIBRE;
	}
	if (((px - 1) >= 0) && ((py - 1) >= 0) && !(j.mina.p[px - 1][py - 1] == SALIDA)) {    //Posicion arriba-izquierda
		j.mina.p[px - 1][py - 1] = LIBRE;
	}
}

void caePiedrasDinamita(tJuego &j, int px, int py) {

	if ((px - 2) >= 0) {
		if (j.mina.p[px - 2][py] == PIEDRA || j.mina.p[px - 2][py] == GEMA) {
			caidaPiedras(j, px - 2, py);
		}
		if ((py - 1) >= 0) {
			if (j.mina.p[px - 2][py - 1] == PIEDRA || j.mina.p[px - 2][py - 1] == GEMA) {
				caidaPiedras(j, px - 2, py - 1);
			}
		}
		if ((py + 1) < j.mina.ncol){
			if (j.mina.p[px - 2][py + 1] == PIEDRA || j.mina.p[px - 2][py + 1] == GEMA) {
				caidaPiedras(j, px - 2, py + 1);
			}
		}
	}

}