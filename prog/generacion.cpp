#include "generacion.h"

generacion::generacion(tinyxml2::XMLDocument &doc, vector<string> typePaths) {
    this->doc = &doc;
    this->typePaths = typePaths;
}

void generacion::update(vector<vector<float>> positionPaths) {
    this->positionPaths = positionPaths;
    generateFile();
}

void generacion::generateFile() {

    tinyxml2::XMLElement* groupPaths = this->doc->NewElement("g");
    tinyxml2::XMLElement* newPath;

    vector<vector<float>> tempPositionPaths = this->positionPaths;
    int positionSelector = 0;
    int moveSelector = 0;

    for (string path : this->typePaths) {

        int charSelector = 0;
        string attributePath = "M ";

        vector<float> valuesX = tempPositionPaths[positionSelector];
        vector<float> valuesY = tempPositionPaths[positionSelector + 1];

        attributePath += to_string(valuesX[valuesX.size() - 1]) + " " + to_string(valuesY[valuesY.size() - 1]) + " ";

        newPath = this->doc->NewElement("path");

        int x_Selector = 1;
        int y_Selector = 1;
        char actualType;

        int x1 = 0;
        int y1 = 0;
        int x2 = 0;
        int y2 = 0;
        int x3 = 0;
        int y3 = 0;

        while (charSelector <= path.size()) {

            actualType = path[charSelector++];
            
            switch (actualType) {
            case 'C':
                x1 = valuesX[x_Selector++];
                y1 = valuesY[y_Selector++];
                x2 = valuesX[x_Selector++];
                y2 = valuesY[y_Selector++];
                x3 = valuesX[x_Selector++];
                y3 = valuesY[y_Selector++];

                attributePath += "C " + to_string(x1) + " " + to_string(y1) + " " +
                                                    to_string(x2) + " " + to_string(y2) + " " +
                                                    to_string(x3) + " " + to_string(y3) + " ";
                break;
            case 'H':
                x1 = valuesX[x_Selector++];
                attributePath += "H " + to_string(x1) + " ";
                break;
            case 'V':
                y1 = valuesY[y_Selector++];
                attributePath += "V " + to_string(y1) + " ";
                break;
            case 'L':
                x1 = valuesX[x_Selector++];
                y1 = valuesY[y_Selector++];
                attributePath += "L " + to_string(x1) + " " + to_string(y1) + " ";
                break;
            }
        }

        attributePath += "Z";
        newPath->SetAttribute("d", attributePath.c_str());
        groupPaths->InsertEndChild(newPath);

        positionSelector += 2;
    }

    string fileTittle = "C:/Users/deyne/Desktop/asd/images/svg/NuevoGenerado-" + to_string((this->fileCounter)++) + ".svg";

    doc->FirstChildElement()->InsertEndChild(groupPaths);
    doc->SaveFile(fileTittle.c_str());

    groupPaths->DeleteChildren();
}



