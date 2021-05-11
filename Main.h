#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <math.h>
using namespace std;

const int TAM_CLASE = 100;
const int TAM_ATRIBUTOS = 4;
const int TAM_TESTS = 3;
const string FICHERO = "Iris2Clases.txt";

typedef float atributos[TAM_ATRIBUTOS];

typedef struct {
	atributos datos[TAM_CLASE];
	string nombreClase;
	int contador;
} tClase;

typedef struct {
	atributos dato;
	string nombreClase;
} tMuestra;

typedef tMuestra tMuestras[TAM_TESTS];

typedef struct {
	tMuestras muestras;
	int contador;
} tLista;

int menu();
void cargar(tClase& irisSe, tClase& irisVer);
void cargarTests(tLista& tests);
void mostrar(tClase irisSe, tClase irisVer);
void mostrarTests(tLista tests);
void bayes(tClase& irisSe, tClase& irisVer, tLista& tests, tLista& centros);
void mediasBayes(tClase& iris, tLista& centros);
void mostrarAlgoritmo(tLista centros, string algoritmo);
void bayesTests(tLista tests, tLista centros);
void lloyd(tClase& irisSe, tClase& irisVer, tLista& tests, tLista& centros);
int menorDistancia(atributos atributos, tLista centros);
bool toleranciaLloyd(atributos centro1, atributos centro2, double tolerancia, tLista centros);
void inicializarCentros(tLista& centros);
void kMedias(tClase& irisSe, tClase& irisVer, tLista& tests, tLista& centros);