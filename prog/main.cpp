
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
	
	doc.LoadFile("../arch_svg/wifi-2.svg");
	
	int puntos[] = {2583, 79};
	int colores[] = {0x0000ff};

	seleccion Seleccion(puntos, 2, colores, 1, doc);
	Seleccion.selectPaths();

	vector<string> typePaths = Seleccion.getTypePath();
	vector<float> limitsPaths = Seleccion.getPathsArea();
	vector<vector<float>> valuesPaths = Seleccion.getPathValues();

	cout << "\n\nProceso Terminado" << endl;

	// tinyxml2::XMLElement* gElemento = doc.FirstChildElement()->FirstChildElement();
	// tinyxml2::XMLElement* centro = doc.NewElement("circle");
	// centro->SetAttribute("cx", 2500);
	// centro->SetAttribute("cy", 1870);
	// centro->SetAttribute("r", "40");
	// centro->SetAttribute("fill", "#000000");
	// gElemento->InsertEndChild(centro);


	// int x = 2500;
	// int y = 1870;

	// int vX = 100;
	// int vY = 0;
	
	// float angulo = 360.0;
	// angulo = (angulo * 3.1415)/180.0;

	// int new_vX = (vX * cos(angulo)) + (vY * sin(angulo));
	// int new_vY = - (vX * sin(angulo)) + (vY * cos(angulo));

	// vX = new_vX;
	// vY = new_vY;

	// for(int i = 0; i <= 100; i++) {
	// 	x += vX;
	// 	y += vY;

	// 	centro = doc.NewElement("circle");
	// 	centro->SetAttribute("cx", x);
	// 	centro->SetAttribute("cy", y);
	// 	centro->SetAttribute("r", "20");
	// 	centro->SetAttribute("fill", "#00ff00");
	// 	gElemento->InsertEndChild(centro);
	// }

	// doc.SaveFile("PruebaBasica.svg");



	return 0;
}
