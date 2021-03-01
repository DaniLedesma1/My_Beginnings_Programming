//Daniel Ledesma Ventura
//Nestor Marin Gomez

#include "mina.h"
#include "juego.h"
#include "puntuaciones.h"
using namespace std;


const int MAX_NIVEL = 4;


/************************************Prototipo**************************************/
int menu();
void mostrarOpciones();
int elegirOpcion();
int otraPartida();
string pedirNombre();
int seleccionarNivel();
int inicioPrograma(tPuntuaciones &m, string nom);
int calcularPuntuacion(tJuego j);

int main() {
	
	int op;
	int nivel = 1;
	int pos, posMina;
	string nom;
	bool seguir = true;

	tJuego j;
	tPuntuaciones m;

	inicializar_Marcador(m);
	cargar_Marcador(m);
	nom = pedirNombre();

	pos = inicioPrograma(m, nom); //Devuelve la posicion del usuario(nuevo o no), ademas de preparar la salida para cualquiera de los 2 casos

	op = menu();
	if (op != 0) {
		j.escala = op; //Guardamos la escala 
	}

	while (op != 0 && nivel != 0) {

		cout << endl << nom << ", Que mina quieres explorar? " << endl;
		nivel = seleccionarNivel();

		if (nivel != 0) {
			cargar_Juego(j, nivel);
			if (j.modoEmpleo == 1) {
				jugar(j, std::cin);
			}
			else {
				ifstream movimientos;
				movimientos.open("movimientos.txt");
				if (movimientos.is_open()) {
					jugar(j, movimientos);
				}
			}

			if (j.estadoPartida == 0) { //Si ha superado el nivel, actualizamos su marcador
				if (m.array_clasificacion[pos].minasRecorridas > 0) {
					if (buscarMina(nivel, m.array_clasificacion[pos], posMina)) {  //Buscamos la mina para ver si existe, y devolvemos en posMina donde esta o donde deberia estar
						actualizarDatos(m, pos, posMina, calcularPuntuacion(j), j.numMovs, j.numGemas, j.numDinamitas);
					}
					else {// Si es una mina nueva, la insertamos
						insertarMina(m.array_clasificacion[pos], nivel, j.numMovs, j.numGemas, j.numDinamitas, calcularPuntuacion(j), posMina);
						m.array_clasificacion[pos].punt_total += calcularPuntuacion(j);
					}
				}
				else { //Si el jugador no tiene aun ninguna mina guardada por ser nuevo jugador, o porque cuando se registro perdio y no guardó resultados
					insertarMina(m.array_clasificacion[pos], nivel, j.numMovs, j.numGemas, j.numDinamitas, calcularPuntuacion(j), 0);
					m.array_clasificacion[pos].punt_total = calcularPuntuacion(j);
				}
			}


			op = otraPartida();
		}
		
	}
	guardar_Marcador(m);
	destruir(m);
	system("pause");
	return 0;
}




int menu() {
	int opcion;
	mostrarOpciones();
	opcion = elegirOpcion();
	return opcion;
}

void mostrarOpciones() {
	cout << "1. Jugar partida a escala 1:1." << endl;
	cout << "2. Jugar partida a escala 1:3." << endl;
	cout << "0. Salir" << endl << endl;
}

int elegirOpcion() {
	int op;
	cout << "Elija una opcion: ";
	cin >> op;
	while (cin.fail() || op > 2) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "Error: opcion escogida no valida. " << endl;
		cout << "Elija una opcion: ";
		cin >> op;
	}
	cin.sync();
	return op;
}


int otraPartida() {
	int op;
	cout << "1. Jugar otra partida." << endl;
	cout << "0. Salir." << endl;
	cout << "Elija una opcion: ";
	cin >> op;
	while (cin.fail() || op > 1) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "Error: opcion escogida no valida. " << endl;
		cout << "Elija una opcion: ";
		cin >> op;
	}
	cin.sync();
	return op;
}

string pedirNombre() {
	string nom;
	cout << "                    Introduce tu nombre de jugador/a: ";
	cin >> nom;
	return nom;
}


int seleccionarNivel() {
	int niv;
	cout << "Introduce un numero entre 1 y 5 para explorar una mina y 0 para salir: ";
	cin >> niv;
	while (cin.fail() || niv < 0 || niv > 5) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "Error: opcion escogida no valida. " << endl;
		cout << "Introduce un numero entre 1 y 5 para explorar una mina y 0 para salir: ";
		cin >> niv;
	}
	return niv;
}

int inicioPrograma(tPuntuaciones &m, string nom) {

	int pos = 0;

	if (buscar(nom, m, pos)) {
		cout << endl << "                       Ya estas registrado / a." << endl << endl;
		cout << "           Mira las minas que has recorrido ordenadas por nivel." << endl << endl;
		mostrar_Minas_Usuario(m, pos);
	}
	else {
		cout << endl << "                           Eres nuevo / a: " << nom << endl << endl;
		cout << "               Mira las puntuaciones de los otros jugadores. " << endl << endl;
		mostrar_Alfabetico(m);
		insertar(m, nom, pos);
	}
	return pos;
}


int calcularPuntuacion(tJuego j) {
	int punt = 0;
	punt = ((j.mina.ncol *j.mina.nfil) + (10 * j.numGemas) - j.numMovs - (2 * j.numDinamitas));
	return punt;
}