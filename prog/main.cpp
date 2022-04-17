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

	int contador = 0;

	while (elementSelector) {
		if (!elementSelector->NoChildren()) {
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

			// Check if is absolute or relative
			if (att_Value[0] == 'M')
				curveTo_Character = 'C';
			else
				curveTo_Character = 'c';

			// Get the move and curve section
			int positionCurveTo = att_Value.find(curveTo_Character);
			moveSection = att_Value.substr(0, positionCurveTo);
			curveSection = att_Value.substr(positionCurveTo, att_Value.length() - positionCurveTo);


			/* --> Converting moves values into coordinates numbers*/
			// Clean not integers chars in the strings
			if (moveSection[0] > '9')
				moveSection.erase(0, 1);
			
			// Convert string values into numbers
			int separatorPosition = moveSection.find(',');
			float Move_x = stof(moveSection.substr(0, separatorPosition));
			float Move_y = stof(moveSection.substr(separatorPosition + 1, moveSection.length() - separatorPosition));

			/* --> Converting curves values into numbers*/
			
			// Save all curves of a single path in a vector
			vector<string> curvesInPath;
			size_t c_Position = 0;
			while (true) {
				c_Position = curveSection.find_first_of("Cc", 1);

				if (c_Position != string::npos) {
					curvesInPath.push_back(curveSection.substr(0, c_Position));
					curveSection.erase(0, c_Position);
				}
				else {
					curvesInPath.push_back(curveSection);
					break;
				}
			}
			
			// Save all points of the curves
			size_t startPoint = 0;
			size_t separator1 = 0;
			size_t separator2 = 0;
			float auxMoveX = 0.0;
			float auxMoveY = 0.0;
			char curveType;
			vector<float> curvePoints_X;
			vector<float> curvePoints_Y;
			
			for (string curve : curvesInPath) {
				startPoint = 0;
				separator1 = 0;
				separator2 = 0;

				curveType = curve[0];
				curve.erase(0, 1);

				if (curveType == 'c') {
					auxMoveX = Move_x;
					auxMoveY = Move_y;
				}

				while (separator2 != string::npos) {
					separator1 = curve.find_first_of(",", startPoint);
					separator2 = curve.find_first_of(",", separator1 + 1);

					int limitPoint2 = (separator2 != string::npos) ? separator2 - separator1 - 1 : curve.length();

					string point1 = curve.substr(startPoint, separator1 - startPoint);
					string point2 = curve.substr(separator1 + 1, limitPoint2);

					curvePoints_X.push_back(abs(stof(point1)) + auxMoveX);
					curvePoints_Y.push_back(abs(stof(point2)) + auxMoveY);

					startPoint = separator2 + 1;
				}
			}

			// Check if there is any user-points inside the curve points
			sort(curvePoints_X.begin(), curvePoints_X.end());
			sort(curvePoints_Y.begin(), curvePoints_Y.end());

			float majorX = curvePoints_X[curvePoints_X.size() - 1];
			float minorX = curvePoints_X[0];
			
			float majorY = curvePoints_Y[curvePoints_Y.size() - 1];
			float minorY = curvePoints_Y[0];

			for (int i = 0; i < puntosSize; i+=2) {

				if ((puntos[i] >= minorX && puntos[i] <= majorX) && (puntos[i+1] >= minorY && puntos[i+1] <= majorY))
					cout << contador++ << ": Path Intersecado " << att_Value << endl;

			}
		
			pathSelector = pathSelector->NextSiblingElement();
		}
	}
	
	return;
}