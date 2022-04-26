
#include "seleccion.cpp"
#include <math.h>
#include <list>
#include <utility>

string listaColorsitos[] = {"#ff0000", "#00ff00", "#0000ff"};

void printSVG(tinyxml2::XMLDocument &doc, tinyxml2::XMLPrinter &printer) {
	// Print in memory
	doc.Print(&printer);
	cout << printer.CStr() << endl;
}

int main() {

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLPrinter printer;
	
	// doc.LoadFile("../arch_svg/wifi-1.svg");
	doc.LoadFile("wifiColores.svg");
	// printSVG(doc, printer);

	int puntos[] = {2500, 1870};
	int colores[] = {0x00ff00};

	seleccion Seleccion(puntos, 2, colores, 1, doc);
	Seleccion.selectPaths();

	return 0;
}
