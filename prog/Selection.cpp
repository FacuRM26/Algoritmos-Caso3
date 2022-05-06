#include "Selection.h"

void Selection::attach(Observer* pRouter) {
    this->current_router = pRouter;
}
void Selection::notify() {
    this->current_router->update();
}

Selection::Selection(int userPoints[], int size_userPoints, int userColors[], int size_userColors, tinyxml2::XMLDocument &doc) {
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
void Selection::getAllPaths(tinyxml2::XMLElement* pElement) {
    if (pElement) {
        if (!string(pElement->Value()).compare("path")) {
            this->paths.push_back(pElement);
        }
        if (!pElement->NoChildren()) {
            getAllPaths(pElement->FirstChildElement());
        }
        getAllPaths(pElement->NextSiblingElement());
    }
}
void Selection::saveRGBColors(int pUserColors[], int pSize) {
    for (int i = 0; i < pSize; i++) {
        int color = pUserColors[i];
        int b = color%256; color = color/256;
        int g = color%256; color = color/256;
        int r = color%256;

        this->userColors.push_back({r, g, b});
    }
}

// Converting moves values into coordinates numbers
void Selection::getMoveValues(string pMoveSection, float *pMove_x, float *pMove_y) {
    // Clean not integers chars in the strings
    if (pMoveSection[0] > '9')
        pMoveSection.erase(0, 1);

    // Convert string values into numbers
    int comaPosition = pMoveSection.find(',');
    *pMove_x = stof(pMoveSection.substr(0, comaPosition));
    *pMove_y = stof(pMoveSection.substr(comaPosition + 1, pMoveSection.length() - comaPosition));
}

// Converting curves values (relative and absolute) into coordinates numbers(absolutes)
void Selection::getCurveValues(string pCurveSection, vector<float> pCurvePoints[], float pMove_x, float pMove_y, string* pTypes) {
    // Search for the positions of the separators and convert the values between them
    string separatorsChars = " -,CcHhVvLlZz";
    size_t separator1 = 0;
    size_t separator2 = 0;
    string point;

    float moveValues[] = {0, 0};
    int axisSelector = 0;
    bool horizontalCase = false;

    separator1 = pCurveSection.find_first_of(separatorsChars, 0);

    while (toupper(pCurveSection[separator1]) != 'Z') {
        separator2 = pCurveSection.find_first_of(separatorsChars, separator1 + 1);
        separator2 = (separator2-1 != separator1) ? separator2 : pCurveSection.find_first_of(separatorsChars, separator2 + 1);

        if (separator2 != string::npos) {

            char currentSeparator = toupper(pCurveSection[separator1]);
            if (currentSeparator >= 'A')
                pTypes->push_back(toupper(pCurveSection[separator1]));

            switch (currentSeparator) {
                case 'H':
                    axisSelector = 0;
                    horizontalCase = true;
                    break;
                case 'V':
                    axisSelector = 1;
                    break;
            }
            if (pCurveSection[separator1] >= 'a') {
                moveValues[0] = pMove_x;
                moveValues[1] = pMove_y;
            }
            else if (pCurveSection[separator1] >= 'A') {
                moveValues[0] = 0;
                moveValues[1] = 0;
            }

            separator1 = (pCurveSection[separator1] == '-') ? separator1 - 1 : separator1;

            point = pCurveSection.substr(separator1 + 1, separator2 - separator1 - 1);
            pCurvePoints[axisSelector].push_back(stof(point) + moveValues[axisSelector]);
            
            if (!horizontalCase)
                axisSelector = (1 - axisSelector);
            else {
                axisSelector = 0;
                horizontalCase = false;
            }
            separator1 = separator2;
        }
        else
            break;
    }
}
// Check if there is any user-points inside the curve points
bool Selection::pathIntersect(vector<float> pCurvePoints[], float* pMajorX, float* pMinorX, float* pMajorY, float* pMinorY) {
    sort(pCurvePoints[0].begin(), pCurvePoints[0].end());
    sort(pCurvePoints[1].begin(), pCurvePoints[1].end());

    *pMajorX = pCurvePoints[0][pCurvePoints[0].size() - 1];
    *pMinorX = pCurvePoints[0][0];

    *pMajorY = pCurvePoints[1][pCurvePoints[1].size() - 1];
    *pMinorY = pCurvePoints[1][0];

    for (int i = 0; i < this->size_userPoints; i += 2) {
        if ((this->userPoints[i] >= *pMinorX && this->userPoints[i] <= *pMajorX) &&
            (this->userPoints[i + 1] >= *pMinorY && this->userPoints[i + 1] <= *pMajorY)) {
            return true;
        }
    }
    return false;
}
// Check if there is any color that matches with the user colors
bool Selection::checkColorIntersection(int pColor) {
    int b = pColor%256; pColor = pColor/256;
	int g = pColor%256; pColor = pColor/256;
	int r = pColor%256;

    int diff1 = 0;
	int diff2 = 0;
	int diff3 = 0;
    float distance = 0.0;

    for (auto userColor : this->userColors) {
        diff1 = abs(r - userColor[0]);
        diff2 = abs(g - userColor[1]);
        diff3 = abs(b - userColor[2]);
        distance = 100.0 - (((float)(diff1 + diff2 + diff3))/(3.0*255.0)*100.0);

        if (distance >= 70)
            return true;
    }
    return false;
}

// Select paths that intersect the user points and save them in a file
void Selection::selectPaths() {
    string att_Value;
	string moveSection;
	string curveSection;
    int color_Value = 0x000000;
	char curveTo_Character = 'c';

    int countPaths = 0;

    for (tinyxml2::XMLElement* pathSelector : paths) {
        // Get the color attribute
        color_Value = 0x000000;
        const tinyxml2::XMLAttribute* fillAttribute = pathSelector->FindAttribute("fill");

        if (fillAttribute) {
            color_Value = stoi(string(fillAttribute->Value()).substr(1, 6), 0, 16);
        }
        if (!checkColorIntersection(color_Value))
            continue;

        // Get the full value of 'd'
		att_Value = pathSelector->FindAttribute("d")->Value();

		// Get the move and curve section
		int positionCurveTo = att_Value.find_first_of("CcHhVvLl");
		moveSection = att_Value.substr(0, positionCurveTo);
		curveSection = att_Value.substr(positionCurveTo, att_Value.length() - positionCurveTo);

        // Get the values of move
        float pMove_x = 0;
        float pMove_y = 0;
        getMoveValues(moveSection, &pMove_x, &pMove_y);

        // Get all the values inside the curve paths
        string typePath_Chars = "";
        vector<float> pCurvePoints[] = {{}, {}};
        getCurveValues(curveSection, pCurvePoints, pMove_x, pMove_y, &typePath_Chars);

        // ------- PRUEBA
        vector<float> tempX = pCurvePoints[0];
        vector<float> tempY = pCurvePoints[1];

        // See if the path intersect some userPoint
        float pMajorX = 0.0;
        float pMinorX = 0.0;
        float pMajorY = 0.0;
        float pMinorY = 0.0;
        if (pathIntersect(pCurvePoints, &pMajorX, &pMinorX, &pMajorY, &pMinorY)) {
            countPaths++;
            
            tempX.push_back(pMove_x);
            tempY.push_back(pMove_y);

            typePath.push_back(typePath_Chars);
            pathValues.push_back(tempX);
            pathValues.push_back(tempY);
            pathsArea.push_back(pMajorX);
            pathsArea.push_back(pMinorX);
            pathsArea.push_back(pMajorY);
            pathsArea.push_back(pMinorY);
        }
    }
    cout << "\nPaths Intersecados: " << countPaths;
}

vector<string> Selection::getTypePath() {
    return this->typePath;
}
vector<float> Selection::getPathsArea() {
    return this->pathsArea;
}
vector<vector<float>> Selection::getPathValues() {
    return this->pathValues;
}


