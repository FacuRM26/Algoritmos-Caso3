
#include "Selection.cpp"
#include "Router.cpp"
#include "Generator.cpp"
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
	float angulo = 270.0;

	cout << "\nCarga del archivo completa!\nIniciando proceso de Selection...\n";
	Selection Selection(puntos, 10, colores, 2, doc);
	Selection.selectPaths();

	cout << "\nProceso de Selection finalizado!\n";
	vector<string> typePaths = Selection.getTypePath();
	vector<vector<float>> valuesPaths = Selection.getPathValues();

	vector<vector<float>> positionPaths;

	Router* router = new Router(doc, valuesPaths, frames, angulo, &positionPaths);
	Generator* generador = new Generator(doc, typePaths, &positionPaths);

	Selection.attach(router);
	router->attach(generador);

	Selection.notify();

	cout << "\nProceso Terminado\n";

	delete router;
	delete generador;

	return 0;
}
