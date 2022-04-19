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

	// // Save the original file
	// doc.SaveFile("/arch_svg/wifi-1.svg");

	// Search for the 'svg' element
	// tinyxml2::XMLElement *svg_Element = doc.FirstChildElement();
	// tinyxml2::XMLElement *g_Element = svg_Element->FirstChildElement();

	// // Create and insert a new element
	// tinyxml2::XMLElement *newPath_Element = g_Element->InsertNewChildElement("path");
	// newPath_Element->SetAttribute("d", "M200 0 L125 200 L275 200 Z");
	// newPath_Element->SetAttribute("fill", "#04aa6d");

	// // Save the file with one new element
	// doc.SaveFile("wifi-2.svg");

	// // Create and insert a new element
	// newPath_Element = g_Element->InsertNewChildElement("path");
	// newPath_Element->SetAttribute("d", "M250 0 L175 200 L325 200 Z");
	// newPath_Element->SetAttribute("fill", "#fcba03");

	// // Save the file with the last new element
	// doc.SaveFile("wifi-3.svg");


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
		vector<string> curves;

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
			vector<string> curvesInPath;
			size_t c_Position = 0;

			vector<vector<int>> separatorsPositions;
			size_t s_Position = 0;
			int sP_Selector = 0;
			bool readingCurves = true;

			while (readingCurves) {
				c_Position = curveSection.find_first_of("Cc", 1);
				string actualCurve = "";
				
				if (c_Position != string::npos) {
					actualCurve = curveSection.substr(0, c_Position);
					curvesInPath.push_back(actualCurve);
					curveSection.erase(0, c_Position);
				}
				else {
					actualCurve = curveSection;
					curvesInPath.push_back(actualCurve);
					readingCurves = false;
				}

				separatorsPositions.push_back({});
				s_Position = actualCurve.find_first_of(" ,-", 0);
				for (; s_Position != string::npos; s_Position = actualCurve.find_first_of(" ,-", s_Position+1)) {
					separatorsPositions[sP_Selector].push_back(s_Position);
				}
				sP_Selector++;
			}

			// Get the real points of the curves
			vector<float> curvePoints[] = {{}, {}};
			int moveValues[] = {0, 0};
			int axisSelector = 0;
			string point;

			for (int i = 0; i < curvesInPath.size(); i++) {
				axisSelector = 0;
				string curve = curvesInPath[i];
				moveValues[0] = 0;
				moveValues[1] = 0;
				
				if (curve[0] == 'c') {
					moveValues[0] = Move_x;
					moveValues[1] = Move_y;
				}
				
				if (curve[1] >= '0') {
					point = curve.substr(1, separatorsPositions[i][0] - 1);
					curvePoints[0].push_back(abs(stof(point)) + moveValues[0]);
					axisSelector = 1;
				}
				
				int j = 0;
				int sizeV_Separators = separatorsPositions[i].size();
				for (; j < sizeV_Separators; j++) {
					int separator1 = separatorsPositions[i][j];
					int separator2 = (j == sizeV_Separators - 1) ? curve.size() : separatorsPositions[i][j + 1];

					switch (curve[separator1]) {
					case '-':
						point = curve.substr(separator1, separator2 - separator1);
						break;
					
					default:
						point = curve.substr(separator1 + 1, separator2 - separator1 - 1);
						break;
					}
					curvePoints[axisSelector].push_back(abs(stof(point)) + moveValues[axisSelector]);
					axisSelector = (1 - axisSelector);
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
					break;
				}
			}
		
			pathSelector = pathSelector->NextSiblingElement();
		}
	}
	return;
}