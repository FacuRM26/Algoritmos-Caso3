
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

void sizeBox(tinyxml2::XMLDocument &doc, string &sizeX, string &sizeY) {
    tinyxml2::XMLElement *pRoot = doc.FirstChildElement("svg");
    string view = pRoot->FindAttribute("viewBox")->Value();
    
    view.erase(0, view.find(' ') + 1);
    view.erase(0, view.find(' ') + 1);
    sizeX = view.substr(0, view.find(' '));
    view.erase(0, view.find(' ') + 1);
    sizeY = view.substr(0, view.find(' '));
}

void calcularDistancia(float angulo, vector<float> pathArea, float scX, float scY, tinyxml2::XMLDocument &doc) {

	float Frames = 50.0;
	int degg = round(angulo * (180.0/3.1415));

	// Tengo el size de la ventana
	float sizeX = scX;
	float sizeY = scY;

	float actualLineX = 0.0;
	float actualLineY = 0.0;

	float referencePoint_X = 0.0;
	float referencePoint_Y = 0.0;

	if (degg >= 0 && degg <= 90) {
		referencePoint_X = pathArea[0];
		referencePoint_Y = pathArea[2];
		actualLineX = sizeX;
		actualLineY = sizeY;
	}
	else if (degg >= 90 && degg <= 180) {
		referencePoint_X = pathArea[1];
		referencePoint_Y = pathArea[2];
		actualLineX = 0.0;
		actualLineY = sizeY;
	}
	else if (degg >= 180 && degg <= 270) {
		referencePoint_X = pathArea[1];
		referencePoint_Y = pathArea[3];
		actualLineX = 0.0;
		actualLineY = 0.0;
	}
	else if (degg >= 270) {
		referencePoint_X = pathArea[0];
		referencePoint_Y = pathArea[3];
		actualLineX = sizeX;
		actualLineY = 0.0;
	}

	// Hacer la formula de la funcion lineal
	float m = tan(angulo);
	float b = referencePoint_Y - (referencePoint_X * m);

	// Busco el punto de interseccion y calcular la distancia
	float interseccionY = 0.0;
	float interseccionX = 0.0;

	float distancia = 0.0;
	float point1 = 0.0;
	float point2 = 0.0;

	if (degg % 90) {
		cout << "Valor m: " << m << "\nValor b: " << b << endl;
		interseccionX = (actualLineY - b) / m;
		interseccionY = (m * actualLineX) + b;

		if (interseccionX < 0)
			interseccionX = abs(interseccionX) + sizeX;
		if (interseccionY < 0)
			interseccionY = abs(interseccionY) + sizeY;

		cout << "Puntos: " << referencePoint_X << " " << referencePoint_Y << endl;
		cout << "Lineas: " << actualLineX << " " << actualLineY << endl;
		cout << "Intersecciones\n";
		cout << interseccionX << endl << interseccionY << endl << endl;

		if (interseccionY < interseccionX) {
			point1 = referencePoint_X - actualLineX;
			point2 = referencePoint_Y - interseccionY;
			distancia = sqrt((point1 * point1) + (point2 * point2));
		}
		else {
			point1 = referencePoint_X - interseccionX;
			point2 = referencePoint_Y - actualLineY;
			distancia = sqrt((point1 * point1) + (point2 * point2));
		}
	}
	else if (degg == 90 || degg == 270){
		distancia = abs(referencePoint_Y - actualLineY);
	}
	else {
		distancia = abs(referencePoint_X - actualLineX);
	}

	float distPerFrame = distancia / Frames;

	// Calcular el vector de direccion
	cout << "Distancia: " << distancia << endl;
	tinyxml2::XMLElement* gEl = doc.FirstChildElement()->FirstChildElement();

	float vX = distPerFrame;
    float vY = 0;

    float new_vX = (vX * cos(angulo)) - (vY * sin(angulo));
    float new_vY = (vX * sin(angulo)) + (vY * cos(angulo));

    cout << new_vX << endl;
    cout << new_vY << endl;

	for (int i = 0; i < Frames; i++) {
		tinyxml2::XMLElement* newCircle = doc.NewElement("circle");

		referencePoint_X += new_vX;
		referencePoint_Y += new_vY;

		newCircle->SetAttribute("cx", referencePoint_X);
		newCircle->SetAttribute("cy", referencePoint_Y);
		newCircle->SetAttribute("r", 40);
		newCircle->SetAttribute("fill", "blue");

		gEl->InsertEndChild(newCircle);
	}

	doc.SaveFile("PruebaNuevoAquiBroXdMilf.svg");

}

int main() {

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLPrinter printer;
	
	if (doc.LoadFile("../arch_svg/ejemplo.svg")) {
		cout << "\nFalla en la lectura del archivo!\n";
		return 0;
	}

	string sizeX = "";
	string sizeY = "";
	sizeBox(doc, sizeX, sizeY);

	vector<float> puntos = {1600.0, 500.0, 2000.0, 1500.0};

	calcularDistancia(6.1086, puntos, stof(sizeX), stof(sizeY), doc);

	// int puntos[] = {2000, 2000, 2500, 3700, 1800, 300, 3605, 1123, 5642, 800};
	// int colores[] = {0xff0000, 0x0000ff};
	// int frames = 50;
	// float angulo = 120.0;

	// cout << "\nCarga del archivo completa!\nIniciando proceso de seleccion...\n";
	// seleccion Seleccion(puntos, 10, colores, 2, doc);
	// Seleccion.selectPaths();

	// cout << "\nProceso de seleccion finalizado!\n";
	// vector<string> typePaths = Seleccion.getTypePath();
	// vector<vector<float>> valuesPaths = Seleccion.getPathValues();
	// vector<float> pathArea = Seleccion.getPathsArea();

	// Routing* routing = new Routing(doc, valuesPaths, frames, angulo);
	// generacion* generador = new generacion(doc, typePaths);

	// Seleccion.attach(routing);
	// routing->attach(generador);

	// Seleccion.notify();

	// cout << "\nProceso Terminado\n";

	// delete routing;
	// delete generador;

	return 0;
}