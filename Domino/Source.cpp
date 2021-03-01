//Daniel Ledesma Ventura
//Nestor Marin Gomez

#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
using namespace std;

const bool FICHAS_OCULTAS = false;
const int NUM_FICHAS = 28;
const int MIN_JUGADORES = 2;
const int MAX_JUGADORES = 4;
const int MIN_PTOS_FICHA = 6;
const int MAX_PTOS_FICHA = 9;


typedef struct {
	int num1;
	int num2;
}tFicha;

typedef tFicha tArrayFichas[NUM_FICHAS];

typedef struct {
	tArrayFichas aFichas;
	int cont;
}tListaFichas;

typedef struct {
	tListaFichas listaFichas;
	int puntos;
}tJugador;

typedef tJugador tArrayJugadores[MAX_JUGADORES];

typedef struct {
	int numJugadores;
	int valorMaxFichas;
	string tablero;
	tListaFichas pozo;
	tArrayJugadores jugs;
	bool iniciado;
	bool ganado;
	int turno;
}tJuego;

/**********************DECLARACION DE LAS FUNCIONES*****************************/

void inicializarJuego(tJuego &juego);
int elegirJugadores();
void generaPozo(tJuego &juego, int maxPuntos);
void desordenaPozo(tArrayFichas pozo);
void iniciarJugadores(tJuego &juego);
tFicha robarFicha(tJuego &juego);
int quienEmpieza(const tJuego juego, short int &indice);
void eliminaFicha(tJuego &juego, short int posJug, short int posFicha);
bool estrategia1(tJuego &juego);
bool estrategia2(tJuego &juego);
bool puedePonerIzq(tFicha &fich, string tablero);
bool puedePonerDer(tFicha &fich, string tablero);
void invertirFicha(tFicha &ficha);
void ponerFichaIzq(tJuego &juego, int jugador, int i);
void ponerFichaDer(tJuego &juego, int jugador, int i);
string estructurar(tFicha fich);
string muestraFicha(string ficha);
void guardarJuego(const tJuego juego);
void guardarListaFichas(ofstream &salida, const tListaFichas listaFichas);
bool cargarJuego(tJuego &juego);;
void cargarListaFichas(ifstream &salida, tListaFichas &listaFichas);
bool sinSalida(const tJuego juego);
bool comprobar(tListaFichas fichas, string tablero);
int elegirOpcion();
void mostrarTablero(tJuego juego, bool FICHAS_OCULTAS);
void tituloTablero();
void mostrarFichas(tListaFichas fichs);
int mostrarMenu();
int opciones();
void acabarPrimeraRonda(tJuego &juego);
bool ejecutarOpcion(short int opcionMenu, tJuego &juego, bool &accionHecha);
void sumaPuntos(tJuego &juego);
void jugarMaquinas(tJuego &juego);
bool comprobarFin(tJuego juego);
bool otraPartida();
int seleccionFicha(short int cont);
bool puesta(tJuego &juego, short int &ficha, int lado);
void mostrarPuntos(const tJuego juego);
/*******************************************************************************/


int main() {
	//Variables
	int op, inicio, i = 1;
	tJuego juego;
	bool seguir = false, otraPart = true, continuar = false, accionHecha = false;
	juego.iniciado = false;
	juego.ganado = false;



	inicio = elegirOpcion();
	if (inicio == 1) { //Cargar partida guardada
		cargarJuego(juego);
		juego.iniciado = true;
	}
	else { //Iniciar nueva partida
		juego.numJugadores = elegirJugadores();
		juego.valorMaxFichas = 6;
		inicializarJuego(juego);
		juego.iniciado = true;
	}

	for (int i = 0; i < juego.numJugadores; i++) {
		juego.jugs[i].puntos = 0; //Inicializamos puntos a 0 
	}

	while (otraPart) { //Mientras queramos seguir jugando

		if (!juego.iniciado) {
			inicializarJuego(juego); 
		}
		//Acabar la ronda inicial y empezar la siguiente ronda de turnos con el jugador usuario
		acabarPrimeraRonda(juego);

		juego.iniciado = true;
		seguir = false;
		continuar = false;
		//Ronda de turnos 2, empezando por el usuario.
		while (!seguir && !continuar) { //Mientras pueda seguir y continuar

			while (!accionHecha) { //Por si el jugador ejecuta una accion que no pueda cumplir
				system("cls");
				mostrarTablero(juego, FICHAS_OCULTAS); //MUESTRO EL TABLERO CON LAS FICHAS
				op = mostrarMenu();

				seguir = ejecutarOpcion(op, juego, accionHecha); //BOOLEANO QUE INDICA SI SIGO O NO PARA SALIR DEL WHILE
				system("cls");
				mostrarTablero(juego, FICHAS_OCULTAS);
			}
			accionHecha = false;
			juego.turno = 0;

			while (i < juego.numJugadores) { //Turno de las maquinas
				juego.turno = (juego.turno + 1) % juego.numJugadores;
				jugarMaquinas(juego);
				i++;
			}
			i = 1;

			continuar = comprobarFin(juego);

		}

		sumaPuntos(juego);
		mostrarPuntos(juego);

		otraPart = otraPartida();
		juego.iniciado = false;
		juego.ganado = false;

	}

	system("pause");
	return 0;

}

//Metodo que permite elegir si cargar o iniciar una nueva partida
int elegirOpcion() {
	int opcion;
	cout << endl << "1. Cargar partida guardada. " << endl;
	cout << "2. Iniciar nueva partida. " << endl;
	cout << "Elija la opcion: ";
	cin >> opcion;
	while (cin.fail() || opcion > 2 || opcion < 1) { //COMPRUEBA QUE SE CORRESPONDE CON LAS OPCIONES HABILITADAS Y NO FALLA

		cin.clear(); cin.ignore(INT_MAX, '\n');
		cout << "Error: opcion escogida no valida. " << endl;
		cout << "Elija la opcion: ";
		cin >> opcion;
	}
	cin.sync();
	return opcion;
}



//Escribe en pantalla toda la información de la partida, incluyendo las fichas del jugador :
void mostrarTablero(tJuego juego, bool FICHAS_OCULTAS) {
	tituloTablero();
	cout << juego.tablero << endl << endl;
	for (int i = 1; i < juego.numJugadores; i++) { //jugador 0 es el usuario
		cout << "Maquina " << "#" << i << "  ";
		if (!FICHAS_OCULTAS) {
			mostrarFichas(juego.jugs[i].listaFichas);
			cout << endl;
		}
		else {
			cout << juego.jugs[i].listaFichas.cont << endl;
		}
	}
	cout << endl << "Jugador     ";
	mostrarFichas(juego.jugs[0].listaFichas);
	cout << endl;
}


void tituloTablero() {
	cout << "-------------------" << endl;
	cout << "|     TABLERO      |" << endl;
	cout << "-------------------" << endl;
}


void mostrarFichas(tListaFichas fichs) {
	for (int i = 0; i < fichs.cont; i++) {
		string aux = estructurar(fichs.aFichas[i]);
		cout << muestraFicha(aux) << " ";
	}

}


int mostrarMenu() {

	int opcion;
	cout << "-------------------" << endl;
	cout << "| MENU DE OPCIONES |" << endl;
	cout << "-------------------" << endl;
	cout << "1. Poner ficha por la izquierda" << endl;
	cout << "2. Poner ficha por la derecha" << endl;
	cout << "3. Robar ficha nueva" << endl;
	cout << "4. Guardar partida" << endl;
	cout << "0. Salir" << endl << endl;
	opcion = opciones();
	return opcion;
}

//Opciones del menu
int opciones() {
	int opcion;
	cout << "Elija la opcion: ";
	cin >> opcion;
	while (cin.fail() || opcion > 4 || opcion < 0) { //COMPRUEBA QUE SE CORRESPONDE CON LAS OPCIONES HABILITADAS Y NO FALLA

		cin.clear(); cin.ignore(INT_MAX, '\n');
		cout << "Error: opcion escogida no valida. " << endl;
		cout << "Elija la opcion: ";
		cin >> opcion;
	}
	cin.sync();
	return opcion;
}


bool ejecutarOpcion(short int opcionMenu, tJuego &juego, bool &accionHecha) {

	short int ficha = 0;
	bool puedoPoner = false;

	if (opcionMenu == 0) { //EL PROGRAMA ACABA CUANDO SE CONVIERTE EN TRUE
		accionHecha = true;
		return true;
	}
	else if (opcionMenu == 1) {

		if (puesta(juego, ficha, 0)) { // si es 0 coloco por la izquierda
			eliminaFicha(juego, 0, ficha);
			accionHecha = true;
		}
		if (juego.jugs[0].listaFichas.cont == 0) {
			juego.ganado = true;
			return true;
		}
		else return false;
	}
	else if (opcionMenu == 2) {

		if (puesta(juego, ficha, 1)) { // si es 1 coloco por la derecha
			eliminaFicha(juego, 0, ficha);
			accionHecha = true;
		}
		if (juego.jugs[0].listaFichas.cont == 0) {
			juego.ganado = true;
			return true;
		}
		else return false;
	}
	else if (opcionMenu == 3) {
		puedoPoner = comprobar(juego.jugs[0].listaFichas, juego.tablero);
		while (!puedoPoner && juego.pozo.cont > 0) {

			juego.jugs[0].listaFichas.aFichas[juego.jugs[0].listaFichas.cont] = robarFicha(juego);
			juego.jugs[0].listaFichas.cont++;

			if (comprobar(juego.jugs[0].listaFichas, juego.tablero)) { //Para ver si ahora con la ficha robada puedo poner alguna ficha, sino sigo robando
				puedoPoner = true;
			}
			system("cls"); //Repito estas dos lineas porque si llego al fin del programa(derrota del usuario) no se muestra su ultima ficha robada
			mostrarTablero(juego, FICHAS_OCULTAS);
			accionHecha = true;

		}
		if (!puedoPoner && juego.pozo.cont == 0) {
			accionHecha = true;
		}

		return false;
	}
	else if (opcionMenu == 4) {

		guardarJuego(juego);
		return false;
	}

}

//Partiendo de un juego sin inicializar, crea la configuración inicial de la partida, en la que ya se ha colocado la primera pieza
//(del jugador que tenía el mayor doble) en el tablero; si nadie lo tiene, se repetirá el reparto hasta que se consiga.En el parámetro
//jugador se devuelve el jugador al que le toca colocar ficha :
void inicializarJuego(tJuego &juego) {
	short int indice, empezar;
	bool continuar = false;
	
	juego.tablero = "";
	while (!continuar) {
		generaPozo(juego, juego.valorMaxFichas);
		desordenaPozo(juego.pozo.aFichas);
		iniciarJugadores(juego);
		empezar = quienEmpieza(juego, indice); //Buscar el jugador que empieza
		if (empezar != -1) {
			continuar = true;
		}
	}
	if (empezar == juego.numJugadores - 1) { //si le toca al ultimo jugador, el turno inicia en el primer jugador
		juego.turno = 0;
	}
	else juego.turno = empezar + 1; //sino empieza el siguiente

	juego.tablero += muestraFicha(estructurar(juego.jugs[empezar].listaFichas.aFichas[indice])); // colocar el doble en el tablero
	eliminaFicha(juego, empezar, indice); // eliminar la ficha doble del jugador

}




//Elegimos los jugadores de la partida
int elegirJugadores() {
	int opcion;
	cout << "Elija cuantos jugadores habran(2-4): ";
	cin >> opcion;
	while (cin.fail() || opcion > MAX_JUGADORES || opcion < MIN_JUGADORES) { //COMPRUEBA QUE SE CORRESPONDE CON LAS OPCIONES HABILITADAS Y NO FALLA

		cin.clear(); cin.ignore(INT_MAX, '\n');
		cout << "Error: opcion escogida no valida. " << endl;
		cout << "Elija la opcion: ";
		cin >> opcion;
	}
	cin.sync();
	return opcion;
}

//Genera en pozo todas las piezas posibles del dominó para fichas con valores de 0 a maxPuntos :
void generaPozo(tJuego &juego, int maxPuntos) {
	int k = 0;
	int cont = 0;
	for (int i = k; i < maxPuntos + 1; i++) {
		for (int j = k; j < maxPuntos + 1; j++) {
			juego.pozo.aFichas[cont].num1 = i;
			juego.pozo.aFichas[cont].num2 = j;
			cont++;
		}
		k++;
	}
	juego.pozo.cont = cont;
}


//Desordena el pozo:
void desordenaPozo(tArrayFichas pozo) {
	int idx1, idx2;
	tArrayFichas aux;
	for (int i = 0; i < 1000; i++) {
		idx1 = rand() % NUM_FICHAS;
		idx2 = rand() % NUM_FICHAS;
		if (idx1 != idx2) {
			aux[0] = pozo[idx1];
			pozo[idx1] = pozo[idx2];
			pozo[idx2] = aux[0];
		}
	}
}

//Inicializa los jugadores de la partida
void iniciarJugadores(tJuego &juego) {
	for (int i = 0; i < juego.numJugadores; i++) {
		juego.jugs[i].listaFichas.cont = 0; //Inicializamos contadores a 0 
		

		for (int j = 0; j < 7; j++) {
			juego.jugs[i].listaFichas.aFichas[j] = robarFicha(juego);
			juego.jugs[i].listaFichas.cont++;
		}
	}
}



//Quita la última ficha del pozo y la devuelve
tFicha robarFicha(tJuego &juego) {
	tFicha fich;
	fich = juego.pozo.aFichas[juego.pozo.cont - 1];
	juego.pozo.cont--;

	return fich;
}

//Devuelve quién empieza (0: humano, >0: máquina, -1: nadie) y, en el índice, la posición de la ficha con la que empezar :
int quienEmpieza(const tJuego juego, short int &indice) {
	int n1, n2;
	int num = -1; //numero del doble para ir comparando los dobles de cada jugador
	int jug; //jugador actual con el mayor doble
	bool encontrado = false; //solo funciona para el doble 6, sino recorreremos los jugadores buscando el mayor doble
	bool valido = false; //para saber si hay algun doble
	int i = 0;
	while (!encontrado && i < juego.numJugadores) {
		for (int j = 0; j < juego.jugs[i].listaFichas.cont; j++) {
			n1 = juego.jugs[i].listaFichas.aFichas[j].num1; //abreviatura
			n2 = juego.jugs[i].listaFichas.aFichas[j].num2;
			if (n1 == n2) {
				if (n1 != 6) {
					if (n1 > num) {
						num = n1;
						jug = i;
						indice = j;
						valido = true;
					}
				}
				else {
					jug = i;
					indice = j;
					encontrado = true;
				}
			}
		}
		i++;
	}
	if (!encontrado && !valido) {
		jug = -1;
	}
	return jug;
}

//Quita de las fichas del jugador su ficha fichaNum-ésima desplazando el resto de posiciones a la izquierda para rellenar el hueco dejado por la ficha quitada :
void eliminaFicha(tJuego &juego, short int posJug, short int posFicha) {
	for (int i = posFicha; i < juego.jugs[posJug].listaFichas.cont; i++) {
		juego.jugs[posJug].listaFichas.aFichas[i] = juego.jugs[posJug].listaFichas.aFichas[i + 1];
	}
	juego.jugs[posJug].listaFichas.cont--;
}



//Realiza el movimiento del jugador máquina correspondiente utilizando la estrategia 1 (y devuelve si logró realizar algún movimiento) :
bool estrategia1(tJuego &juego) {
	bool puede = false;
	int i = 0;
	while (i < juego.jugs[juego.turno].listaFichas.cont && !puede) {
		if (puedePonerIzq(juego.jugs[juego.turno].listaFichas.aFichas[i], juego.tablero)) { //Comprobamos por izquierda si se puede colocar
			ponerFichaIzq(juego, juego.turno, i);
			eliminaFicha(juego, juego.turno, i);
			puede = true;
			if (juego.jugs[juego.turno].listaFichas.cont == 0) { //Miramos si ha ganado la partida
				juego.ganado = true;
			}
		}

		else if (puedePonerDer(juego.jugs[juego.turno].listaFichas.aFichas[i], juego.tablero)) { //Comprobamos por derecha
			ponerFichaDer(juego, juego.turno, i);
			eliminaFicha(juego, juego.turno, i);
			puede = true;
			if (juego.jugs[juego.turno].listaFichas.cont == 0) {
				juego.ganado = true;
			}
		}
		else i++;
	}
	return puede;
}

//Lo propio para la estrategia 2:
bool estrategia2(tJuego &juego) {
	bool valIzq = false, valDer = false, puede = false;
	int num1, num2, pos;
	if (juego.jugs[1].listaFichas.cont > 1) {
		int valorMasAlto = 0;
		
		for (int i = 0; i < juego.jugs[1].listaFichas.cont; i++) { //Recorremos las fichas de la maquina
			num1 = juego.jugs[1].listaFichas.aFichas[i].num1;
			num2 = juego.jugs[1].listaFichas.aFichas[i].num2;
			if (puedePonerIzq(juego.jugs[1].listaFichas.aFichas[i], juego.tablero) && (valorMasAlto < (num1 + num2))) { //Si suma mas que el valor anterior que se podia poner 
				valorMasAlto = num1 + num2;																				//y se puede poner se guarda esa ficha	
				pos = i;
				valIzq = true;
				valDer = false; // por si acaso ya se habia activado antes
				puede = true;
			}

			else if (puedePonerDer(juego.jugs[1].listaFichas.aFichas[i], juego.tablero) && (valorMasAlto < (num1 + num2))) {
				valorMasAlto = num1 + num2;
				pos = i;
				valIzq = false;
				valDer = true;
				puede = true;
			}
			
		}
		if (valIzq) {
			ponerFichaIzq(juego, 1, pos);
			eliminaFicha(juego, 1, pos);
		}
		else if (valDer) {
			ponerFichaDer(juego, 1, pos);
			eliminaFicha(juego, 1, pos);
		}
	}
	else {
		if (puedePonerIzq(juego.jugs[1].listaFichas.aFichas[0], juego.tablero)) {
			ponerFichaIzq(juego, 1, 0);
			eliminaFicha(juego, 1, 0);
			juego.ganado = true;
			puede = true;
		}

		else if (puedePonerDer(juego.jugs[1].listaFichas.aFichas[0], juego.tablero)) {
			ponerFichaDer(juego, 1, 0);
			eliminaFicha(juego, 1, 0);
			juego.ganado = true;
			puede = true;
		}
	}

	return puede;
}



bool puedePonerIzq(tFicha &fich, string tablero) { //COMPRUEBA SI ES POSIBLE COLOCAR A LA IZQUIERDA UNA FICHA
	if (tablero[1] - 48 == fich.num2) { //Considerar estructura del tablero tal que empieza con |num1-num2 
		return true;
	}
	else if (tablero[1] - 48 == fich.num1) {
		invertirFicha(fich);
		return true;
	}
	else { return false; }
}

bool puedePonerDer(tFicha &fich, string tablero) { //COMPRUEBA SI ES POSIBLE COLOCAR A LA DERECHA UNA FICHA
	if (tablero[tablero.size() - 2] - 48 == fich.num1) { // Lo mismo pero ahora hay que coger la penultima posicion del tablero para ver el numero y obviar |
		return true;
	}
	else if (tablero[tablero.size() - 2] - 48 == fich.num2) {
		invertirFicha(fich);
		return true;
	}
	else { return false; }
}

void invertirFicha(tFicha &ficha) {
	int aux;
	aux = ficha.num1;
	ficha.num1 = ficha.num2;
	ficha.num2 = aux;
}

void ponerFichaIzq(tJuego &juego, int jugador, int i) { //COLOCA A LA IZQUIERDA UNA FICHA

	string aux = estructurar(juego.jugs[jugador].listaFichas.aFichas[i]);
	juego.tablero = muestraFicha(aux) + juego.tablero; //SE ANIADE LA FICHA AL TABLERO

}

void ponerFichaDer(tJuego &juego, int jugador, int i) { //COLOCA A LA DERECHA UNA FICHA

	string aux = estructurar(juego.jugs[jugador].listaFichas.aFichas[i]);
	juego.tablero += muestraFicha(aux); //SE ANIADE LA FICHA AL TABLERO

}

string estructurar(tFicha fich) { // para ponerle la estructura num1-num2
	return to_string(fich.num1) + "-" + to_string(fich.num2);
}

string muestraFicha(string ficha) { //DEVUELVE LA FICHA EN EL FORMATO ADECUADO PARA EL TABLERO
	return "|" + ficha + "|";
}

//Guardar la partida:
void guardarJuego(const tJuego juego) {
	string arc;
	cout << "Dame el nombre del archivo(sin el .txt): ";
	cin >> arc;

	ofstream archivo;
	archivo.open(arc + ".txt"); //ABRIR ARCHIVO
	if (archivo.is_open()) { //INTRODUCIR DATOS NECESARIOS PARA CARGAR UNA PARTIDA POSTERIORMENTE
		archivo << juego.numJugadores << " " << juego.valorMaxFichas << endl;
		archivo << juego.tablero << endl;
		archivo << juego.pozo.cont << endl;
		for (int i = 0; i < juego.pozo.cont; i++) {
			archivo << juego.pozo.aFichas[i].num1 << " " << juego.pozo.aFichas[i].num2 << " ";
		}
		archivo << endl;
		for (int j = 0; j < juego.numJugadores; j++) {
			guardarListaFichas(archivo, juego.jugs[j].listaFichas);
			archivo << endl << juego.jugs[j].puntos << endl;
		}

		archivo.close();
	}
}

//Guarda las fichas en el siguiente renglón del archivo de salida:
void guardarListaFichas(ofstream &salida, const tListaFichas listaFichas) {
	salida << listaFichas.cont << endl;
	for (int i = 0; i < listaFichas.cont; i++) {
		salida << listaFichas.aFichas[i].num1 << " " << listaFichas.aFichas[i].num2 << " ";
	}

}

//Carga la partida:
bool cargarJuego(tJuego &juego) {
	string arc;
	cout << "Dame el nombre del archivo(sin el .txt): ";
	cin >> arc;
	ifstream archivo;
	archivo.open(arc + ".txt"); //ABRIR ARCHIVO
	if (!archivo.is_open()) {
		cout << "No existe ese archivo txt o no se ha podido abrir" << endl;
	}
	else { //UNA VEZ ABIERTO, CARGAMOS LOS VALORES PARA UTILIZARLOS
		archivo >> juego.numJugadores >> juego.valorMaxFichas;
		archivo >> juego.tablero;
		archivo >> juego.pozo.cont;
		for (int i = 0; i < juego.pozo.cont; i++) {
			archivo >> juego.pozo.aFichas[i].num1 >> juego.pozo.aFichas[i].num2; 
		}
		for (int j = 0; j < juego.numJugadores; j++) {
			cargarListaFichas(archivo, juego.jugs[j].listaFichas);
			archivo >> juego.jugs[j].puntos;
		}


		archivo.close();
	}
	return true;
}

//Carga las fichas del siguiente renglón del archivo de entrada:
void cargarListaFichas(ifstream &entrada, tListaFichas &listaFichas) {
	entrada >> listaFichas.cont;
	for (int i = 0; i < listaFichas.cont; i++) {
		entrada >> listaFichas.aFichas[i].num1 >> listaFichas.aFichas[i].num2;
	}
}

//Indica si el pozo se ha quedado sin piezas y, además, ningún jugador puede colocar ninguna de sus piezas :
bool sinSalida(const tJuego juego) {
	bool pozo = false, poner = false;
	int i = 0;
	if (juego.pozo.cont == 0) {
		pozo = true;
	}
	while (i < juego.numJugadores && !poner && !(juego.jugs[i].listaFichas.cont == 0)) {
		poner = comprobar(juego.jugs[i].listaFichas, juego.tablero);
		i++;
	}
	if (pozo && !poner) {
		return true;
	}
	else return false;

}



//Comprueba que no se pueda colocar ninguna ficha del jugador tanto por la izquierda como por la derecha
bool comprobar(tListaFichas fichas, string tablero) {
	bool algunLado = false;
	int i = 0;

	while (i < fichas.cont && !algunLado) {
		if (puedePonerIzq(fichas.aFichas[i], tablero) || puedePonerDer(fichas.aFichas[i], tablero)) {
			algunLado = true;
		}
		else i++;
	}
	return algunLado;
}




//Comprueba que la ficha seleccionada se puede colocar en el lado indicado y se ha colocado.
bool puesta(tJuego &juego, short int &ficha, int lado) {
	bool puesto = false;
	ficha = seleccionFicha(juego.jugs[0].listaFichas.cont);

	if (lado == 0) {
		if (puedePonerIzq(juego.jugs[0].listaFichas.aFichas[ficha], juego.tablero)) {
			ponerFichaIzq(juego, 0, ficha);
			puesto = true;
		}
	}
	else if (lado == 1) {
		if (puedePonerDer(juego.jugs[0].listaFichas.aFichas[ficha], juego.tablero)) {
			ponerFichaDer(juego, 0, ficha);
			puesto = true;
		}
	}
	return puesto;
}


int seleccionFicha(short int cont) {
	int op;
	cout << "Elija la pieza que desea colocar: ";
	cin >> op;
	while (cin.fail() || op > cont) {
		cin.clear(); cin.ignore(INT_MAX, '\n');
		cout << "Error: opcion escogida no valida. " << endl;
		cout << "Elija la opcion: ";
		cin >> op;
	}
	cin.sync();
	return op;
}


//Suma los puntos acumulados en todas las fichas del jugador:
void sumaPuntos(tJuego &juego) {
	int num1, num2, acumulativo = 0;
	for (int i = 0; i < juego.numJugadores; i++) {
		for (int j = 0; j < juego.jugs[i].listaFichas.cont; j++) {
			num1 = juego.jugs[i].listaFichas.aFichas[j].num1;
			num2 = juego.jugs[i].listaFichas.aFichas[j].num2;
			acumulativo += num1 + num2;
		}
		if (acumulativo == 0) {
			juego.jugs[i].puntos += acumulativo;
			cout << endl << endl << "El jugador " << i << " es el vencedor de la ronda." << endl;
			acumulativo = 0;
		}
		else {
			juego.jugs[i].puntos += acumulativo;
			acumulativo = 0;
		}
	}
}



bool otraPartida() {
	char op;
	cout << endl << "Desea jugar otra partida? (S-N) ";
	cin >> op;
	while (cin.fail() && op != 'N' && op != 'S') {
		cin.clear(); cin.ignore(INT_MAX, '\n');
		cout << "Error: opcion escogida no valida. " << endl;
		cout << "Elija la opcion: (S-N) ";
		cin >> op;
	}
	cin.sync();
	if (op == 'S') {
		return true;
	}
	else return false;
}

//Acaba la ronda inicial para partir del usuario el resto del juego
void acabarPrimeraRonda(tJuego &juego) {
	if (juego.turno == 1) {
		estrategia2(juego);
		system("cls");
		mostrarTablero(juego, FICHAS_OCULTAS);
		int i = juego.turno + 1;
		while (i < juego.numJugadores) {
			juego.turno++;
			estrategia1(juego);
			system("cls");
			mostrarTablero(juego, FICHAS_OCULTAS);
			i++;
		}
	}
	else if (juego.turno == 2) {
		estrategia1(juego);
		system("cls");
		mostrarTablero(juego, FICHAS_OCULTAS);
		if (juego.numJugadores == 4) {
			juego.turno++;
			estrategia1(juego);
			system("cls");
			mostrarTablero(juego, FICHAS_OCULTAS);
		}
	}
	else if (juego.turno == 3) {
		estrategia1(juego);
		system("cls");
		mostrarTablero(juego, FICHAS_OCULTAS);
	}
	juego.turno = 0; //empezamos siguiente ronda por el usuario
}


void jugarMaquinas(tJuego &juego) {
	bool coloca = false, puedoPoner = false;

	if (juego.iniciado && !juego.ganado && juego.turno != 0)

	{

		if (juego.turno == 1) coloca = estrategia2(juego);

		else coloca = estrategia1(juego);

		if (!coloca) {
			while (!puedoPoner && juego.pozo.cont > 0) {

				juego.jugs[juego.turno].listaFichas.aFichas[juego.jugs[juego.turno].listaFichas.cont] = robarFicha(juego);
				juego.jugs[juego.turno].listaFichas.cont++;

				if (comprobar(juego.jugs[juego.turno].listaFichas, juego.tablero)) { //Para ver si ahora con la ficha robada puedo poner alguna ficha, sino sigo robando
					puedoPoner = true;
				}
				system("cls");
				mostrarTablero(juego, FICHAS_OCULTAS);

			}

			
		}
		system("cls"); // Borrar la pantalla

		mostrarTablero(juego, FICHAS_OCULTAS);

	}
	puedoPoner = comprobar(juego.jugs[juego.turno].listaFichas, juego.tablero);
	
}

//Comprueba si es el fin de la partida
bool comprobarFin(tJuego juego) {
	
	if (juego.ganado == true) {
		return true;
	}
	else if (sinSalida(juego)) {
		cout << endl << "Partida bloqueada, sin salida" << endl;
		return true;
	}
	else return false;
}

void mostrarPuntos(const tJuego juego) {
	for (int i = 0; i < juego.numJugadores; i++) {
		cout << endl << "Los puntos del Jugador " << i << " son: " << juego.jugs[i].puntos << endl;
	}
}
