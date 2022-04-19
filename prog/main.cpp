#include "../tinyXML/tinyxml2.cpp"
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void seleccion(int puntos[], int puntosSize, int colores[], int coloresSize, tinyxml2::XMLDocument &doc);

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
	int puntos[] = {1592, 272, 1930, 163, 2184, 376, 1748, 440, 2285, 145};
	int colores[] = {0};

	seleccion(puntos, 10, colores, 1, doc);

	return 0;
}


void seleccion(int puntos[], int puntosSize, int colores[], int coloresSize, tinyxml2::XMLDocument &doc) {

	// Search for the 'g' element
	tinyxml2::XMLElement* elementSelector;
	elementSelector = doc.FirstChildElement()->FirstChildElement();

	string elemento = "g";
	int contador = 0;

	while (elementSelector) {
		if (!string(elementSelector->Value()).compare("g")) {
			break;
		}
		elementSelector = elementSelector->NextSiblingElement();
	}

	// Scan all the paths in 'g'
	if (elementSelector) {
		tinyxml2::XMLElement* pathSelector = elementSelector->FirstChildElement();
		string att_Value;
		string moveSection;
		string curveSection;
		char curveTo_Character = 'c';

		while (pathSelector) {
			// Get the full value of 'd'
			att_Value = pathSelector->FindAttribute("d")->Value();

			// Get the move and curve section
			int positionCurveTo = att_Value.find_first_of("Cc");
			moveSection = att_Value.substr(0, positionCurveTo);
			curveSection = att_Value.substr(positionCurveTo, att_Value.length() - positionCurveTo);


			/* --> Converting moves values into coordinates numbers*/
			// Clean not integers chars in the strings
			if (moveSection[0] > '9')
				moveSection.erase(0, 1);
			
			// Convert string values into numbers
			int comaPosition = moveSection.find(',');
			float Move_x = stof(moveSection.substr(0, comaPosition));
			float Move_y = stof(moveSection.substr(comaPosition + 1, moveSection.length() - comaPosition));

			/* --> Converting curves values into numbers*/
			// Save all curves of a single path in a vector
			// Save the position of the separators
			
			int start_cPos = 1;
			int startPosition = 0;
			size_t c_Position = 0;
			size_t separator1 = 0;
			size_t separator2 = 0;
			string point;

			vector<float> curvePoints[] = {{}, {}};
			float moveValues[] = { (curveSection[c_Position]/99) * Move_x, (curveSection[c_Position]/99) * Move_y };
			int axisSelector = 0;
			char actualCurve = curveSection[c_Position];

			while (true) {

				c_Position = curveSection.find_first_of("Cc", start_cPos);
				separator1 = curveSection.find_first_of(" -,", startPosition);
				separator2 = curveSection.find_first_of(" -,", separator1 + 1);

				separator2 = (separator2 == string::npos) ? curveSection.length() : separator2;

				if (separator1 == string::npos)
					break;

				startPosition = separator1 + 1;
				if (separator2 > c_Position) {
					
					actualCurve = curveSection[c_Position];
					curveSection[c_Position] = ' ';

					startPosition = c_Position;
					separator2 = c_Position;
					start_cPos = c_Position + 1;
				}
				
				switch (curveSection[separator1]) {
					case '-':
						point = curveSection.substr(separator1, separator2 - separator1);
						break;
					
					default:
						point = curveSection.substr(separator1 + 1, separator2 - separator1 - 1);
						break;
				}
				curvePoints[axisSelector].push_back(abs(stof(point)) + moveValues[axisSelector]);
				axisSelector = (1 - axisSelector);

				if (curveSection[c_Position] == ' ') {
					moveValues[0] = (actualCurve/99) * Move_x;
					moveValues[1] = (actualCurve/99) * Move_y;
				}
			}

			// Check if there is any user-points inside the curve points
			sort(curvePoints[0].begin(), curvePoints[0].end());
			sort(curvePoints[1].begin(), curvePoints[1].end());

			float majorX = curvePoints[0][curvePoints[0].size() - 1];
			float minorX = curvePoints[0][0];
			
			float majorY = curvePoints[1][curvePoints[1].size() - 1];
			float minorY = curvePoints[1][0];

			contador++;
			for (int i = 0; i < puntosSize; i+=2) {

				if ((puntos[i] >= minorX && puntos[i] <= majorX) && (puntos[i+1] >= minorY && puntos[i+1] <= majorY)) {
					cout << "\n" << contador << ": Path Intersecado\n";
					cout << "\tMajor X: " << majorX;
					cout << "\tPunto X: " << puntos[i];
					cout << "\tMinor X: " << minorX << endl;
					cout << "\tMajor Y: " << majorY;
					cout << "\tPunto Y: " << puntos[i+1];
					cout << "\tMinor Y: " << minorY << endl;
				}
			}
		
			pathSelector = pathSelector->NextSiblingElement();
		}
	}
	return;
}