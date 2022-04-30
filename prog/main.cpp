
#include "seleccion.cpp"
#include "routing.cpp"
#include "generacion.cpp"
#include <math.h>
#include <list>
#include <utility>
#include <queue>


void printSVG(tinyxml2::XMLDocument &doc, tinyxml2::XMLPrinter &printer) {
	// Print in memory
	doc.Print(&printer);
	cout << printer.CStr() << endl;
}

int main() {

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLPrinter printer;
	
	if (doc.LoadFile("../arch_svg/wifiColores.svg")) {
		cout << "\nFalla en la lectura del archivo!\n";
		return 0;
	}

	int puntos[] = {2000, 2000, 2500, 3700, 1800, 300, 3605, 1123, 5642, 800};
	int colores[] = {0xff0000, 0x0000ff};
	int frames = 50;
	float angulo = 120.0;

	cout << "\nCarga del archivo completa!\nIniciando proceso de seleccion...\n";
	seleccion Seleccion(puntos, 10, colores, 2, doc);
	Seleccion.selectPaths();

	cout << "\nProceso de seleccion finalizado!\n";
	vector<string> typePaths = Seleccion.getTypePath();
	vector<vector<float>> valuesPaths = Seleccion.getPathValues();

	vector<vector<float>> positionPaths;

	Routing* routing = new Routing(doc, valuesPaths, frames, angulo, &positionPaths);
	generacion* generador = new generacion(doc, typePaths, &positionPaths);

	Seleccion.attach(routing);
	routing->attach(generador);

	Seleccion.notify();

	cout << "\nProceso Terminado\n";

	delete routing;
	delete generador;

	return 0;
}
