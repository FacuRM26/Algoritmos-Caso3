#include "seleccion.h"

seleccion::seleccion(int userPoints[], int size_userPoints, int userColors[], int size_userColors, tinyxml2::XMLDocument &doc) {
    this->userPoints = userPoints;
    this->size_userPoints = size_userPoints;
    this->doc = &doc;

    // Save all the paths in a vector
    tinyxml2::XMLElement* elementSelector;
	elementSelector = this->doc->FirstChildElement();
    getAllPaths(elementSelector);

    // Save all the colors into RGB values
    saveRGBColors(userColors, size_userColors);
}

// Save all the paths in a vector
void seleccion::getAllPaths(tinyxml2::XMLElement* element) {
    if (element) {
        if (!string(element->Value()).compare("path")) {
            this->paths.push_back(element);
        }
        if (!element->NoChildren()) {
            getAllPaths(element->FirstChildElement());
        }
        getAllPaths(element->NextSiblingElement());
    }
}
void seleccion::saveRGBColors(int userColors[], int size) {
    for (int i = 0; i < size; i++) {
        int color = userColors[i];
        int b = color%256; color = color/256;
        int g = color%256; color = color/256;
        int r = color%256;

        this->userColors.push_back({r, g, b});
    }
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
    string separatorsChars = " -,CcHhVvLlZz";
    size_t separator1 = 0;
    size_t separator2 = 0;
    string point;

    float moveValues[] = {0, 0};
    int axisSelector = 0;
    bool horizontalCase = false;

    separator1 = curveSection.find_first_of(separatorsChars, 0);

    while (toupper(curveSection[separator1]) != 'Z') {
        separator2 = curveSection.find_first_of(separatorsChars, separator1 + 1);
        separator2 = (separator2-1 != separator1) ? separator2 : curveSection.find_first_of(separatorsChars, separator2 + 1);        

        switch (toupper(curveSection[separator1])) {
            case 'H':
                axisSelector = 0;
                horizontalCase = true;
                break;
            case 'V':
                axisSelector = 1;
                break;
        }
        if (curveSection[separator1] >= 'a') {
            moveValues[0] = Move_x;
            moveValues[1] = Move_y;
        }
        else if (curveSection[separator1] >= 'A') {
            moveValues[0] = 0;
            moveValues[1] = 0;
        }

        separator1 = (curveSection[separator1] == '-') ? separator1 - 1 : separator1;

        point = curveSection.substr(separator1 + 1, separator2 - separator1 - 1);
        curvePoints[axisSelector].push_back(stof(point) + moveValues[axisSelector]);
        
        if (!horizontalCase)
            axisSelector = (1 - axisSelector);
        else {
            axisSelector = 0;
            horizontalCase = false;
        }
        separator1 = separator2;
    }
}
// Check if there is any user-points inside the curve points
bool seleccion::pathIntersect(vector<float> curvePoints[]) {
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
// Check if there is any color that matches with the user colors
bool seleccion::checkColorIntersection(int color) {
    int b = color%256; color = color/256;
	int g = color%256; color = color/256;
	int r = color%256;

    int diff1 = 0;
	int diff2 = 0;
	int diff3 = 0;
    float distance = 0.0;

    for (auto userColor : this->userColors) {
        diff1 = abs(r - userColor[0]);
        diff2 = abs(g - userColor[1]);
        diff3 = abs(b - userColor[2]);
        distance = 100.0 - (((float)(diff1 + diff2 + diff3))/(3.0*255.0)*100.0);

        if (distance >= 0.5)
            return true;
    }

    return false;
}

// Select paths that intersect the user points and save them in a file
void seleccion::selectPaths() {
    string att_Value;
	string moveSection;
	string curveSection;
    int color_Value = 0x000000;
	char curveTo_Character = 'c';

    for (tinyxml2::XMLElement* pathSelector : paths) {
        // Get the color attribute
        color_Value = 0x000000;
        if (pathSelector->FindAttribute("color"))
            color_Value = stoi(pathSelector->FindAttribute("color")->Value(), 0, 16);
        if (!checkColorIntersection(color_Value))
            continue;

        // Get the full value of 'd'
		att_Value = pathSelector->FindAttribute("d")->Value();

		// Get the move and curve section
		int positionCurveTo = att_Value.find_first_of("CcHhVvLl");
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
            pathsIntersected.push_back(pathSelector);
        }

    }
    this->doc->SaveFile("nuevoModificado.svg");
}


