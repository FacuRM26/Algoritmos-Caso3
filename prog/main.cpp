
#include "seleccion.cpp"


void printSVG(tinyxml2::XMLDocument &doc, tinyxml2::XMLPrinter &printer) {
	// Print in memory
	doc.Print(&printer);
	cout << printer.CStr() << endl;
}
int main() {

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLPrinter printer;
	
	doc.LoadFile("../arch_svg/wifi-1.svg");
	//printSVG(doc, printer);

	int puntos[] = {1712,2834};
	int colores[] = {0};

	seleccion Seleccion(puntos, 2, doc);
	Seleccion.selectPaths();

	return 0;
}
