#include "selection.cpp"
#include "router.cpp"
#include "generation.cpp"

int main() {

	int userPoints[] = {120, 590, 1000, 600, 500, 530, 230, 600, 700, 700};
	int sizeUserPoints = 10;
	int userColors[] = {0x07bed1, 0xe3e3e3, 0x3b3b3b};
	int sizeUserColors = 3;
	int frames = 50;
	float anglee = 4.7123;
	string fileRoute = "../arch_svg/tesla.svg";


	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(fileRoute.c_str())) {
		cout << "\nFalla en la lectura del archivo!\n";
		return 0;
	}

	Selection selection(userPoints, sizeUserPoints, userColors, sizeUserColors, doc);
	Router* router = new Router(doc, frames, anglee);
	Generation* generation = new Generation(doc, fileRoute);

	selection.attach(router);
	router->attach(generation);

	selection.selectPaths();

	cout << "\nProceso Terminado\n";

	delete router;
	delete generation;

	return 0;
}