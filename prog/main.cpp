
#include "seleccion.cpp"
#include <math.h>
#include <list>
#include <utility>


void printSVG(tinyxml2::XMLDocument &doc, tinyxml2::XMLPrinter &printer) {
	// Print in memory
	doc.Print(&printer);
	cout << printer.CStr() << endl;
}


int main() {

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLPrinter printer;
	
	if (doc.LoadFile("../arch_svg/wifi-2.svg")) {
		cout << "\nFalla en la lectura del archivo!\n";
		return 0;
	}

	int puntos[] = {2000, 2000, 2500, 3700, 1800, 300, 3605, 1123, 5642, 800};
	int colores[] = {0x0000ff, 0x00ff00};

	cout << "\nCarga del archivo completa!\nIniciando proceso de seleccion...\n";
	seleccion Seleccion(puntos, 10, colores, 2, doc);
	Seleccion.selectPaths();


	cout << "\nProceso de seleccion finalizado!\n";
	vector<string> typePaths = Seleccion.getTypePath();
	vector<float> limitsPaths = Seleccion.getPathsArea();
	vector<vector<float>> valuesPaths = Seleccion.getPathValues();

	cout << "\nProceso Terminado\n";

	return 0;
}
