//Daniel Ledesma Ventura
//Nestor Marin Gomez
#ifndef PUNTUACIONES_H
#define PUNTUACIONES_H
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
using namespace std;


const int NUM_TOTAL_MINAS = 5; 
const int CAP_INICIAL = 2;

typedef struct {
	int IdMina;
	int numMovimientos;
	int numGemas;
	int numDinamitas;
	int puntosMina;
} tDatosMina;


typedef struct {
	string nombre;
	int punt_total;
	int minasRecorridas;
	tDatosMina vMinasRecorridas[NUM_TOTAL_MINAS];
} tPuntuacionJugador;


typedef struct {
	int capacidad;
	int num_jugadores;
	tPuntuacionJugador *array_clasificacion;
} tPuntuaciones;


bool cargar_Marcador(tPuntuaciones& marcador);
bool guardar_Marcador(tPuntuaciones marcador);
void mostrar_Minas_Usuario(const tPuntuaciones  marcador, int cont);
void mostrar_Alfabetico(const tPuntuaciones  marcador);
void mostrar_Datos_Usuario(const tPuntuaciones marcador);
void inicializar_Marcador(tPuntuaciones& marcador);
void aumentar_Capacidad(tPuntuaciones& marcador);
void destruir(tPuntuaciones& marcador);
bool buscar(const string nombre, const tPuntuaciones marcador, int& pos);
bool buscarMina(const int id, const tPuntuacionJugador j, int& pos);
void insertar(tPuntuaciones& marcador, string const nombre, int pos);
void insertarMina(tPuntuacionJugador &p, int id, int numMovs, int numGemas, int numDin, int puntosMina, int pos);
void actualizarDatos(tPuntuaciones &m, int posJugador, int posMina, int puntosNuevos, int numMovs, int numGemas, int numDin);

#endif