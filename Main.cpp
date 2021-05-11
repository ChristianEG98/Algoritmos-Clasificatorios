#include "Main.h"

int main() {
	tClase irisSe, irisVer;
	tLista tests, centros;
	centros.contador = 0;
	cargar(irisSe, irisVer);
	cargarTests(tests);
	switch (menu()) {
		case 1: kMedias(irisSe, irisVer, tests, centros); break;
		case 3: lloyd(irisSe, irisVer, tests, centros); break;
		default: bayes(irisSe, irisVer, tests, centros); break;
	}
	system("pause");
	return 0;
}

int menu() {
	int op = 0;
	do {
		cout << "1. Agrupamiento borroso (K-medias)" << endl;
		cout << "2. Bayes" << endl;
		cout << "3. Algoritmo de Lloyd" << endl;
		cout << "Elija una opcion: ";
		cin >> op;
	} while (op < 1 || op > 3);
	return op;
}
void cargar(tClase& irisSe, tClase& irisVer) {
	string lectura;
	atributos atributosDatos;
	irisSe.nombreClase = "Iris-setosa"; irisVer.nombreClase = "Iris-versicolor";
	irisSe.contador = 0; irisVer.contador = 0;

	ifstream archivo;
	archivo.open(FICHERO);
	if (archivo.is_open()) {
		while(!archivo.eof()){
			for (int i = 0; i < TAM_ATRIBUTOS; i++) {
				getline(archivo, lectura, ',');
				atributosDatos[i] = stof(lectura);
			}
			archivo >> lectura;
			if (lectura == "Iris-setosa") {
				for (int i = 0; i < TAM_ATRIBUTOS; i++) irisSe.datos[irisSe.contador][i] = atributosDatos[i];
				irisSe.contador++;
			}
			else {
				for (int i = 0; i < TAM_ATRIBUTOS; i++) irisVer.datos[irisVer.contador][i] = atributosDatos[i];
				irisVer.contador++;
			}
		}
		archivo.close();
	}
	else cout << "No se ha podido abrir el fichero." << endl;
}
void cargarTests(tLista& tests) {
	string lectura;
	atributos atributosDatos;
	tests.contador = 0;

	ifstream archivo;
	for (int i = 0; i < TAM_TESTS; i++) {
		archivo.open("TestIris0" + to_string(i+1) + ".txt");
		if (archivo.is_open()) {
			for (int i = 0; i < TAM_ATRIBUTOS; i++) {
				getline(archivo, lectura, ',');
				atributosDatos[i] = stof(lectura);
			}
			archivo >> tests.muestras[tests.contador].nombreClase;
			for (int i = 0; i < TAM_ATRIBUTOS; i++) tests.muestras[tests.contador].dato[i] = atributosDatos[i];
			tests.contador++;
			archivo.close();
		}
		else cout << "No se ha podido abrir el fichero de prueba numero " + i << endl;
	}
}
void mostrar(tClase irisSe, tClase irisVer) {
	cout << irisSe.nombreClase + ":" << endl;
	for (int i = 0; i < irisSe.contador; i++) {
		for (int j = 0; j < TAM_ATRIBUTOS; j++) {
			cout << setprecision(1) << fixed << irisSe.datos[i][j] << "  ";
		}
		cout << endl;
	}
	cout << endl << irisVer.nombreClase + ":" << endl;
	for (int i = 0; i < irisVer.contador; i++) {
		for (int j = 0; j < TAM_ATRIBUTOS; j++) {
			cout << setprecision(1) << fixed << irisVer.datos[i][j] << "  ";
		}
		cout << endl;
	}
	cout << endl;
}
void mostrarTests(tLista tests) {
	cout << endl;
	for (int i = 0; i < tests.contador; i++) {
		cout << "Test " + to_string(i+1) <<  ": ";
		for (int j = 0; j < TAM_ATRIBUTOS; j++) {
			cout << setprecision(1) << fixed << tests.muestras[i].dato[j] << "  ";
		}
		cout << tests.muestras[i].nombreClase << endl;
	}
	cout << endl;
}
void bayes(tClase& irisSe, tClase& irisVer, tLista& tests, tLista& centros) {
	mediasBayes(irisSe, centros);
	mediasBayes(irisVer, centros);
	mostrarAlgoritmo(centros, "Bayes");
	bayesTests(tests, centros);
}
void mediasBayes(tClase& iris, tLista& centros) {
	atributos clases = { 0, 0, 0, 0 };
	centros.muestras[centros.contador].nombreClase = iris.nombreClase;
	for (int i = 0; i < TAM_ATRIBUTOS; i++) {
		for (int j = 0; j < iris.contador; j++) {
			clases[i] += iris.datos[j][i];
		}
		clases[i] = clases[i] / iris.contador;
	}
	for (int i = 0; i < TAM_ATRIBUTOS; i++) {
		centros.muestras[centros.contador].dato[i] = clases[i];
	}
	centros.contador++;
}
void mostrarAlgoritmo(tLista centros, string algoritmo) {
	cout << endl << "Centros de " << algoritmo << ":" << endl;
	for (int i = 0; i < centros.contador; i++) {
		cout << setw(15) << centros.muestras[i].nombreClase << " | ";
		for (int j = 0; j < TAM_ATRIBUTOS; j++) {
			cout << fixed << setprecision(2) << centros.muestras[i].dato[j] << " ";
		}
		cout << "|" << endl;
	}
}
void bayesTests(tLista tests, tLista centros) {
	atributos xMenosM;
	float distancias[2] = { 0,0 };
	cout << endl;
	for (int i = 0; i < tests.contador; i++) {
		for (int a = 0; a < centros.contador; a++) {
			for (int x = 0; x < TAM_ATRIBUTOS; x++) {
				xMenosM[x] = centros.muestras[a].dato[x] - tests.muestras[i].dato[x];
				distancias[a] =+ xMenosM[x] * xMenosM[x];
			}
		}
		if (distancias[0] < distancias[1]) {
			cout << "El test " << to_string(i + 1) << " pertenece a la clase: " << centros.muestras[0].nombreClase << " -> ";
			if (centros.muestras[0].nombreClase == tests.muestras[i].nombreClase) cout << "Coincide la clase" << endl;
			else cout << "No coincide la clase" << endl;
		}

		else {
			cout << "El test " << to_string(i + 1) << " pertenece a la clase: " << centros.muestras[1].nombreClase << " -> ";
			if (centros.muestras[1].nombreClase == tests.muestras[i].nombreClase) cout << "Coincide la clase" << endl;
			else cout << "No coincide la clase" << endl;
		}
	}
	cout << endl;
}
void lloyd(tClase& irisSe, tClase& irisVer, tLista& tests, tLista& centros) {
	bool fin = false;
	atributos centrosC1, centrosC2;
	int iteraciones = 10;
	float razonAp = 0.1;
	double tolerancia = pow(10, -10);
	inicializarCentros(centros);
	mostrarAlgoritmo(centros, "inicio");
	do {
		for (int i = 0; i < TAM_ATRIBUTOS; i++) {
			centrosC1[i] = centros.muestras[0].dato[i];
			centrosC2[i] = centros.muestras[1].dato[i];
		}
		for (int i = 0; i < irisSe.contador; i++) {
			if (menorDistancia(irisSe.datos[i], centros) == 1) {
				for (int a = 0; a < TAM_ATRIBUTOS; a++) {
					centros.muestras[0].dato[a] = centros.muestras[0].dato[a] + (razonAp * (irisSe.datos[i][a] - centros.muestras[0].dato[a]));
				}
			}
			else {
				for (int a = 0; a < TAM_ATRIBUTOS; a++) {
					centros.muestras[1].dato[a] = centros.muestras[1].dato[a] + (razonAp * (irisSe.datos[i][a] - centros.muestras[1].dato[a]));
				}
			}
		}
		for (int i = 0; i < irisVer.contador; i++) {
			if (menorDistancia(irisVer.datos[i], centros) == 1) {
				for (int a = 0; a < TAM_ATRIBUTOS; a++) {
					centros.muestras[0].dato[a] = centros.muestras[0].dato[a] + (razonAp * (irisVer.datos[i][a] - centros.muestras[0].dato[a]));
				}
			}
			else {
				for (int a = 0; a < TAM_ATRIBUTOS; a++) {
					centros.muestras[1].dato[a] = centros.muestras[1].dato[a] + (razonAp * (irisVer.datos[i][a] - centros.muestras[1].dato[a]));
				}
			}
		}
		if (!toleranciaLloyd(centrosC1, centrosC2, tolerancia, centros)) fin = true;
		iteraciones--;
	} while (!fin && iteraciones > 0);
	mostrarAlgoritmo(centros, "Lloyd");
	cout << endl << "Numero de iteraciones: " << to_string(10 - iteraciones) << endl;
	bayesTests(tests, centros);
}
int menorDistancia(atributos atributos, tLista centros) {
	int clase = 0;
	float distancias[2] = { 0,0 };
	for (int i = 0; i < centros.contador; i++) {
		for (int j = 0; j < TAM_ATRIBUTOS; j++) {
			distancias[i] = pow((atributos[j] - centros.muestras[i].dato[j]), 2);
		}
		distancias[i] = sqrt(distancias[i]);
	}
	if (distancias[0] < distancias[1]) clase = 1;
	else clase = 2;
	return clase;
}
bool toleranciaLloyd(atributos centro1, atributos centro2, double tolerancia, tLista centros) {
	bool continuarIterando = false;
	float distancias[2] = { 0,0 };
	for (int i = 0; i < TAM_ATRIBUTOS; i++) {
		distancias[0] = pow((centro1[i] - centros.muestras[0].dato[i]), 2);
	}
	for (int i = 0; i < TAM_ATRIBUTOS; i++) {
		distancias[1] = pow((centro2[i] - centros.muestras[1].dato[i]), 2);
	}
	for (int i = 0; i < 2; i++) {
		distancias[i] = sqrt(distancias[i]);
		if (distancias[i] < tolerancia || distancias[i] == 0) continuarIterando = false;
		else continuarIterando = true;
	}
	return continuarIterando;
}
void inicializarCentros(tLista& centros) {
	atributos p1 = { 4.6, 3.0, 4.0, 0.0 };
	atributos p2 = { 6.8, 3.4, 4.6, 0.7 };
	centros.contador = 2;
	centros.muestras[0].nombreClase = "Iris-setosa";
	centros.muestras[1].nombreClase = "Iris-versicolor";
	for (int j = 0; j < TAM_ATRIBUTOS; j++) {
		centros.muestras[0].dato[j] = p1[j];
	}
	for (int j = 0; j < TAM_ATRIBUTOS; j++) {
		centros.muestras[1].dato[j] = p2[j];
	}
}
void kMedias(tClase& irisSe, tClase& irisVer, tLista& tests, tLista& centros) {

}