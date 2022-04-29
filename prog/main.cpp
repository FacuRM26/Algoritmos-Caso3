#include "seleccion.cpp"
#include "Routing.cpp"
#include <iostream>
#include <list>
#include <algorithm>
#include <cmath>
#include <vector>
#include <utility>

using namespace std;

int main()
{

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLPrinter printer;

	doc.LoadFile("../arch_svg/wifi-1.svg");

	int puntos[] = {2583, 79};
	int colores[] = {0x000000};

	seleccion Seleccion(puntos, 2, colores, 1, doc);
	Seleccion.selectPaths();

	vector<string> typePaths = Seleccion.getTypePath();
	vector<float> limitsPaths = Seleccion.getPathsArea();
	vector<vector<float>> valuesPaths = Seleccion.getPathValues();
	for(int i=0;i<valuesPaths.size();i+=2){
		cout<<valuesPaths[i].size()<<endl;
		cout<<valuesPaths[i+1].size()<<endl;
		cout<<endl;
	}
	Routing routing(doc,valuesPaths);
	auto coordenadas=routing.calculate_route(100,360);
		for(int i=0;i<coordenadas.size();i+=2){
		cout<<coordenadas[i].size()<<endl;
		cout<<coordenadas[i+1].size()<<endl;
		cout<<endl;
	}
	
	



	return 0;
}