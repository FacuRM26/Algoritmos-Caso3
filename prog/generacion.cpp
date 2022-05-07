#include "generacion.h"

generacion::generacion(tinyxml2::XMLDocument &doc) {
    this->doc = &doc;
}

void generacion::update(vector<Path*> pPathsIntersected) {
    this->pathsIntersected = pPathsIntersected;
    generateFile();
}

string generacion::generateAttribute(int* x_Selector, int* y_Selector, vector<float> valuesX, vector<float> valuesY, char typePath) {
    int x1 = 0; int y1 = 0;
    int x2 = 0; int y2 = 0;
    int x3 = 0; int y3 = 0;

    string attributePath = "";

    switch (typePath) {
    case 'C':
        x1 = valuesX[(*x_Selector)++];
        y1 = valuesY[(*y_Selector)++];
        x2 = valuesX[(*x_Selector)++];
        y2 = valuesY[(*y_Selector)++];
        x3 = valuesX[(*x_Selector)++];
        y3 = valuesY[(*y_Selector)++];

        attributePath = "C " + to_string(x1) + " " + to_string(y1) + " " +
                                            to_string(x2) + " " + to_string(y2) + " " +
                                            to_string(x3) + " " + to_string(y3) + " ";
        break;
    case 'H':
        x1 = valuesX[(*x_Selector)++];
        attributePath = "H " + to_string(x1) + " ";
        break;
    case 'V':
        y1 = valuesY[(*y_Selector)++];
        attributePath = "V " + to_string(y1) + " ";
        break;
    case 'L':
        x1 = valuesX[(*x_Selector)++];
        y1 = valuesY[(*y_Selector)++];
        attributePath = "L " + to_string(x1) + " " + to_string(y1) + " ";
        break;
    }
    return attributePath;
}

void generacion::generateFile() {

    tinyxml2::XMLElement* groupPaths = this->doc->NewElement("g");
    tinyxml2::XMLElement* newPath;

    int moveSelector = 0;
    int pathSelector = 0;

    while (pathSelector < pathsIntersected.size()) {

        newPath = this->doc->NewElement("path");

        int charSelector = 0;
        string attributePath = "";

        string colorPath = pathsIntersected[pathSelector]->getColor();
        vector<float> valuesX = pathsIntersected[pathSelector]->getPathsX();
        vector<float> valuesY = pathsIntersected[pathSelector]->getPathsY();
        string path = pathsIntersected[pathSelector]->getTypePaths();

        float Move_x = valuesX[valuesX.size() - 1];
        float Move_y = valuesY[valuesY.size() - 1];

        attributePath = "M " + to_string(Move_x) + " " + to_string(Move_y) + " ";

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

            if ((charSelector - 1) == path.size()/2) {
                switch (actualType) {
                    case 'C':
                        x_Selector += 3;
                        y_Selector += 3;
                    break;
                    
                    case 'H':
                        x_Selector++;
                    break;
                    
                    case 'V':
                        y_Selector++;
                    break;
                    
                    case 'L':
                        x_Selector++;
                        y_Selector++;
                    break;
                }
                continue;
            }
            attributePath += generateAttribute(&x_Selector, &y_Selector, valuesX, valuesY, actualType);
        }

        attributePath += "Z";
        newPath->SetAttribute("d", attributePath.c_str());

        string att_Transform = "";
        att_Transform += "scale(0.5) " ;
        att_Transform += "translate(" + to_string(Move_x) + ", " + to_string(Move_y) + ")";
        newPath->SetAttribute("transform", att_Transform.c_str());
        newPath->SetAttribute("fill", colorPath.c_str());

        groupPaths->InsertEndChild(newPath);

        pathSelector++;
    }

    string fileTittle = "C:/Users/deyne/Desktop/asd/images/svg/NuevoGenerado-" + to_string((this->fileCounter)++) + ".svg";

    doc->FirstChildElement()->InsertEndChild(groupPaths);
    doc->SaveFile(fileTittle.c_str());

    groupPaths->DeleteChildren();
}



