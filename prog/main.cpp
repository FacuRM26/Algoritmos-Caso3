
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
	int colores[] = {0x00ff00};
	int frames = 50;
	float angulo = 120.0;


	seleccion Seleccion(puntos, 10, colores, 1, doc);
	Routing* routing = new Routing(doc, frames, angulo);
	generacion* generador = new generacion(doc);

	Seleccion.attach(routing);
	routing->attach(generador);

	Seleccion.selectPaths();

	cout << "\nProceso Terminado\n";

	delete routing;
	delete generador;

	return 0;
}