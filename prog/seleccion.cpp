#include "seleccion.h"

seleccion::seleccion(int userPoints[], int size_userPoints, tinyxml2::XMLDocument &doc) {
    this->userPoints = userPoints;
    this->size_userPoints = size_userPoints;
    this->doc = &doc;
}

// Converting moves values into coordinates numbers
void seleccion::getMoveValues(string moveSection, float *Move_x, float *Move_y) {
    // Clean not integers chars in the strings
    if (moveSection[0] > '9')
        moveSection.erase(0, 1);

    // Convert string values into numbers
    int comaPosition = moveSection.find(',');
    *Move_x = stof(moveSection.substr(0, comaPosition));
    *Move_y = stof(moveSection.substr(comaPosition + 1, moveSection.length() - comaPosition));
}

// Converting curves values (relative and absolute) into coordinates numbers(absolutes)
void seleccion::getCurveValues(string curveSection, vector<float> curvePoints[], float Move_x, float Move_y) {
    // Search for the positions of the separators and convert the values between them
    int start_cPos = 1;
    int startPosition = 0;
    size_t c_Position = 0;
    size_t separator1 = 0;
    size_t separator2 = 0;
    string point;

    float moveValues[] = {(curveSection[c_Position] / 99) * Move_x, (curveSection[c_Position] / 99) * Move_y};
    int axisSelector = 0;
    char actualCurve = curveSection[c_Position];

    while (true) {
        c_Position = curveSection.find_first_of("Cc", start_cPos);
        separator1 = curveSection.find_first_of(" -,", startPosition);
        separator2 = curveSection.find_first_of(" -,", separator1 + 1);

        separator2 = (separator2 == string::npos) ? curveSection.length() : separator2;

        if (separator1 == string::npos)
            break;

        if (curveSection[0] != ' ' && (curveSection[1] >= '0' && curveSection[1] <= '9')) {
            separator2 = separator1;
            separator1 = 0;
            curveSection[0] = ' ';
        }

        startPosition = separator1 + 1;
        if (separator2 > c_Position) {

            actualCurve = curveSection[c_Position];
            curveSection[c_Position] = ' ';

            startPosition = c_Position;
            separator2 = c_Position;
            start_cPos = c_Position + 1;
        }

        point = curveSection.substr(separator1 + 1, separator2 - separator1 - 1);
        curvePoints[axisSelector].push_back(abs(stof(point)) + moveValues[axisSelector]);
        axisSelector = (1 - axisSelector);

        if (curveSection[c_Position] == ' ') {
            moveValues[0] = (actualCurve / 99) * Move_x;
            moveValues[1] = (actualCurve / 99) * Move_y;
        }
    }
}
bool seleccion::pathIntersect(vector<float> curvePoints[]) {
    // Check if there is any user-points inside the curve points
    sort(curvePoints[0].begin(), curvePoints[0].end());
    sort(curvePoints[1].begin(), curvePoints[1].end());

    float majorX = curvePoints[0][curvePoints[0].size() - 1];
    float minorX = curvePoints[0][0];

    float majorY = curvePoints[1][curvePoints[1].size() - 1];
    float minorY = curvePoints[1][0];

    for (int i = 0; i < this->size_userPoints; i += 2) {
        if ((this->userPoints[i] >= minorX && this->userPoints[i] <= majorX) && (this->userPoints[i + 1] >= minorY && this->userPoints[i + 1] <= majorY)) {
            return true;
        }
    }
    return false;
}

// Select paths that intersect the user points and save them in a file
void seleccion::selectPaths() {
    // Search for the 'g' element
	tinyxml2::XMLElement* elementSelector;
	elementSelector = this->doc->FirstChildElement()->FirstChildElement();

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

            // Get the values of move
            float Move_x = 0;
            float Move_y = 0;
            getMoveValues(moveSection, &Move_x, &Move_y);

            // Get all the values inside the curve paths
            vector<float> curvePoints[] = {{}, {}};
            getCurveValues(curveSection, curvePoints, Move_x, Move_y);

            // See if the path intersect some userPoint
            if (pathIntersect(curvePoints)) {
                pathSelector->SetAttribute("fill", "#00ee66");
            }
            pathSelector = pathSelector->NextSiblingElement();
        }
    }
    this->doc->SaveFile("nuevoModificado.svg");
}


