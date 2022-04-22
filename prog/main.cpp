
#include "seleccion.cpp"
#include <math.h>

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

	int puntos[] = {2500, 3500};
	int colores[] = {0xeb4034, 0x17218f, 0xd4c62c, 0x54f54e};

	seleccion Seleccion(puntos, 2, colores, 4, doc);
	Seleccion.selectPaths();

	// int color1 = 0x03dbfc;
	// int b1 = color1%256; color1 = color1/256;
	// int g1 = color1%256; color1 = color1/256;
	// int r1 = color1%256;

	// //3, 219, 252
	// cout << r1 <<endl;
	// cout << g1 <<endl;
	// cout << b1 <<endl;

	// cout << "----" << endl;

	// int color2 = 0xffffff;
	// int r2 = 0xff;
	// int g2 = 0xff;
	// int b2 = 0xff;

	// int diff1 = abs(r1 - r2);
	// int diff2 = abs(g1 - g2);
	// int diff3 = abs(b1 - b2);

	// cout << diff1 << endl;
	// cout << diff2 << endl;
	// cout << diff3 << endl;

	// float distance = 100.0 - (((float)(diff1 + diff2 + diff3))/(3.0*255.0)*100.0);

	// cout << distance << endl;

	return 0;
}
