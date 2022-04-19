
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
	
	int puntos[] = {2065,3638, 1712,2834, 1183,2187, 3791,2442, 1359, 187};
	int colores[] = {0};

	seleccion Seleccion(puntos, 10, doc);
	Seleccion.selectPaths();

	return 0;
}
