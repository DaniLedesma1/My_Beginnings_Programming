//Daniel Ledesma Ventura
//Nestor Marin Gomez
#include "puntuaciones.h"

bool cargar_Marcador(tPuntuaciones& marcador) {
	tDatosMina m;
	string centinela = "000";
	string nombre;
	ifstream archivo;
	archivo.open("Puntuaciones.txt");
	if (archivo.is_open()) {
		archivo >> nombre;
		while (nombre != centinela) {
			marcador.array_clasificacion[marcador.num_jugadores].nombre = nombre;
			archivo >> marcador.array_clasificacion[marcador.num_jugadores].punt_total;
			archivo >> marcador.array_clasificacion[marcador.num_jugadores].minasRecorridas;
			for (int i = 0; i < marcador.array_clasificacion[marcador.num_jugadores].minasRecorridas; i++) {
				m = marcador.array_clasificacion[marcador.num_jugadores].vMinasRecorridas[i];
				archivo >> m.IdMina >> m.numMovimientos >> m.numGemas >> m.numDinamitas >> m.puntosMina;
				marcador.array_clasificacion[marcador.num_jugadores].vMinasRecorridas[i] = m;
			}
			marcador.num_jugadores++;
			if (marcador.num_jugadores == marcador.capacidad - 1) {
				aumentar_Capacidad(marcador);
			}
			archivo >> nombre;
		}
		archivo.close();
		return true;
	}
	else {
		cout << "El archivo Puntuaciones.txt no se abrio correctamente" << endl;
		return false;
	}
	
}


bool guardar_Marcador(tPuntuaciones marcador) {

	tDatosMina m;
	string centinela = "000";
	string nombre;
	ofstream archivo;
	archivo.open("Puntuaciones.txt");
	if (archivo.is_open()) {
		for (int i = 0; i < marcador.num_jugadores; i++) {
			archivo << marcador.array_clasificacion[i].nombre << endl;
			archivo << marcador.array_clasificacion[i].punt_total << endl;
			archivo << marcador.array_clasificacion[i].minasRecorridas << endl;
			for(int j = 0; j < marcador.array_clasificacion[i].minasRecorridas; j++){
				m = marcador.array_clasificacion[i].vMinasRecorridas[j];
				archivo << m.IdMina << " " << m.numMovimientos << " " << m.numGemas << " " << m.numDinamitas << " " << m.puntosMina << endl;
			}
		}
		archivo << centinela;
		archivo.close();
		return true;
	}
	else {
		cout << "El archivo Puntuaciones.txt no se abrio correctamente" << endl;
		return false;
	}
}


void mostrar_Minas_Usuario(const tPuntuaciones  marcador, int cont) {

	tDatosMina m;
	
	cout << marcador.array_clasificacion[cont].nombre << "      " << "Movimientos     " << "Gemas     " << "Dinamitas     " << "Puntos    " << "Puntos en total" << endl;

	for (int i = 0; i < marcador.array_clasificacion[cont].minasRecorridas; i++) {
		m = marcador.array_clasificacion[cont].vMinasRecorridas[i];
		if (i == 0) {
			cout << "Mina  " << m.IdMina << setw(13) << m.numMovimientos << setw(13) << m.numGemas << setw(12) << m.numDinamitas << setw(13) << m.puntosMina << setw(14) << marcador.array_clasificacion[cont].punt_total << endl;
		}
		else {
			cout << "Mina  " << m.IdMina << setw(13) << m.numMovimientos << setw(13) << m.numGemas << setw(12) << m.numDinamitas << setw(13) << m.puntosMina << endl;
		}
		
	}
	cout << endl;
}


void mostrar_Alfabetico(const tPuntuaciones  marcador) {

	cout << endl << "          -------------LISTA DE JUGADORES-----------------" << endl << endl; 
	for (int i = 0; i < marcador.num_jugadores; i++) {
		cout << setw(34) << marcador.array_clasificacion[i].nombre << " " << marcador.array_clasificacion[i].punt_total << endl;
	}
	cout << endl << endl;
}


void mostrar_Datos_Usuario(const tPuntuaciones marcador) {

	cout << endl << "            ------- JUGADORES ORDENADOS POR NOMBRE -------" << endl << endl;
	
	for (int i = 0; i < marcador.num_jugadores; i++) {
		mostrar_Minas_Usuario(marcador, i);
	}
}


void inicializar_Marcador(tPuntuaciones &marcador) {
	marcador.num_jugadores = 0;
	marcador.capacidad = CAP_INICIAL;
	marcador.array_clasificacion = new tPuntuacionJugador[CAP_INICIAL];
}


void aumentar_Capacidad(tPuntuaciones& marcador) {

	tPuntuacionJugador * aux = new tPuntuacionJugador[2 * marcador.capacidad];

	for (int i = 0; i < marcador.num_jugadores; i++) {
		aux[i] = marcador.array_clasificacion[i];
	}

	delete[] marcador.array_clasificacion;
	marcador.array_clasificacion = aux;
	marcador.capacidad *= 2;
	aux = nullptr;
}


void destruir(tPuntuaciones& marcador) {
	delete[] marcador.array_clasificacion;
}


bool buscar(const string nombre, const tPuntuaciones marcador, int& pos) {

	int ini = 0;
	int fin = marcador.num_jugadores - 1;
	int mitad;
	bool encontrado = false;

	while((ini <= fin) && !encontrado) {
		mitad = (ini + fin) / 2;
		if(nombre == marcador.array_clasificacion[mitad].nombre){
			encontrado = true;
		}
		else if(nombre < marcador.array_clasificacion[mitad].nombre){
			fin = mitad - 1;
		}
		else{
			ini = mitad + 1;
		}
	}
	if(encontrado)
		pos = mitad;
	else
		pos = ini;
	
	return encontrado;
}


bool buscarMina(const int id, const tPuntuacionJugador j, int& pos) {

	int ini = 0;
	int fin = j.minasRecorridas - 1;
	int mitad;
	bool encontrado = false;

	while ((ini <= fin) && !encontrado) {
		mitad = (ini + fin) / 2;
		if (id == j.vMinasRecorridas[mitad].IdMina) {
			encontrado = true;
		}
		else if (id < j.vMinasRecorridas[mitad].IdMina) {
			fin = mitad - 1;
		}
		else {
			ini = mitad + 1;
		}
	}
	if (encontrado)
		pos = mitad;
	else
		pos = ini;

	return encontrado;
}


void insertar(tPuntuaciones& marcador, string const nombre, int pos){
	marcador.num_jugadores++;
	for (int i = marcador.num_jugadores-1; i > pos; i--) {
		marcador.array_clasificacion[i] = marcador.array_clasificacion[i - 1];
	}
	marcador.array_clasificacion[pos].nombre = nombre;
	marcador.array_clasificacion[pos].minasRecorridas = 0;
	marcador.array_clasificacion[pos].punt_total = 0;

}


void insertarMina(tPuntuacionJugador &p, int id, int numMovs, int numGemas, int numDin, int puntosMina, int pos) {

	p.minasRecorridas++;

	for (int i = p.minasRecorridas - 1; i > pos; i--) {
		p.vMinasRecorridas[i] = p.vMinasRecorridas[i - 1];
	}
	p.vMinasRecorridas[pos].IdMina = id;
	p.vMinasRecorridas[pos].numMovimientos = numMovs;
	p.vMinasRecorridas[pos].numGemas = numGemas;
	p.vMinasRecorridas[pos].numDinamitas = numDin;
	p.vMinasRecorridas[pos].puntosMina = puntosMina;
}


void actualizarDatos(tPuntuaciones &m, int posJugador, int posMina,  int puntosNuevos, int numMovs, int numGemas, int numDin) {

	int puntAux = 0;

	puntAux = m.array_clasificacion[posJugador].vMinasRecorridas[posMina].puntosMina;
	m.array_clasificacion[posJugador].vMinasRecorridas[posMina].puntosMina = puntosNuevos;
	m.array_clasificacion[posJugador].punt_total += (puntosNuevos - puntAux);

	m.array_clasificacion[posJugador].vMinasRecorridas[posMina].numMovimientos = numMovs;
	m.array_clasificacion[posJugador].vMinasRecorridas[posMina].numGemas = numGemas;
	m.array_clasificacion[posJugador].vMinasRecorridas[posMina].numDinamitas = numDin;
}