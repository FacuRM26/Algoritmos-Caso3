#include "generation.h"

Generation::Generation(tinyxml2::XMLDocument &pDoc, string pFileName) {
    this->doc = &pDoc;

    int position1 = pFileName.find_last_of(".");
    int position2 = pFileName.find_last_of("/");

    this->fileName = pFileName.substr(position2 + 1, position1 - position2 - 1);
}

void Generation::update(vector<Path*> pPathsIntersected) {
    this->pathsIntersected = pPathsIntersected;
    finishGeneratePath = false;

    thread threadFileGenerator(&Generation::generateFile, this);

    generatePath();
    threadFileGenerator.join();
}

string Generation::generateAttribute(int* pX_Selector, int* pY_Selector, vector<float> pValuesX, vector<float> pValuesY, char pTypePath) {
    int x1 = 0; int y1 = 0;
    int x2 = 0; int y2 = 0;
    int x3 = 0; int y3 = 0;

    string attributePath = "";

    switch (pTypePath) {
    case 'C':
        x1 = pValuesX[(*pX_Selector)++];    y1 = pValuesY[(*pY_Selector)++];
        x2 = pValuesX[(*pX_Selector)++];    y2 = pValuesY[(*pY_Selector)++];
        x3 = pValuesX[(*pX_Selector)++];    y3 = pValuesY[(*pY_Selector)++];

        attributePath = "C " + to_string(x1) + " " + to_string(y1) + " " +
                               to_string(x2) + " " + to_string(y2) + " " +
                               to_string(x3) + " " + to_string(y3) + " ";
        break;
    case 'H':
        x1 = pValuesX[(*pX_Selector)++];
        attributePath = "H " + to_string(x1) + " ";
        break;
    case 'V':
        y1 = pValuesY[(*pY_Selector)++];
        attributePath = "V " + to_string(y1) + " ";
        break;
    case 'L':
        x1 = pValuesX[(*pX_Selector)++];
        y1 = pValuesY[(*pY_Selector)++];
        attributePath = "L " + to_string(x1) + " " + to_string(y1) + " ";
        break;
    }
    return attributePath;
}

void Generation::generatePath() {
    tinyxml2::XMLElement* newPath;
    int pathSelector = 0;

    while (pathSelector < pathsIntersected.size()) {
        newPath = this->doc->NewElement("path");

        int charSelector = 0;
        string attributePath = "";
        string att_Transform = "";

        string colorPath = pathsIntersected[pathSelector]->getColor();
        vector<float> valuesX = pathsIntersected[pathSelector]->getPathsX();
        vector<float> valuesY = pathsIntersected[pathSelector]->getPathsY();
        string path = pathsIntersected[pathSelector]->getTypePaths();

        float Move_x = valuesX[valuesX.size() - 1];
        float Move_y = valuesY[valuesY.size() - 1];

        attributePath = "M " + to_string(Move_x) + " " + to_string(Move_y) + " ";

        int x_Selector = 0;
        int y_Selector = 0;
        char currentTypePath;

        while (charSelector <= path.size()) {

            currentTypePath = path[charSelector++];

            if ((charSelector - 1) == path.size()/2) {
                switch (currentTypePath) {
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
            attributePath += generateAttribute(&x_Selector, &y_Selector, valuesX, valuesY, currentTypePath);
        }

        attributePath += "Z";
        att_Transform += "scale(0.5) ";
        att_Transform += "translate(" + to_string(Move_x) + ", " + to_string(Move_y) + ")";
        newPath->SetAttribute("d", attributePath.c_str());
        newPath->SetAttribute("transform", att_Transform.c_str());
        newPath->SetAttribute("fill", colorPath.c_str());

        this->queuePaths.push(newPath);
        
        pathSelector++;
    }
    finishGeneratePath = true;
}

void Generation::generateFile() {

    string newFileName = this->fileName + to_string((this->fileCounter)++) + ".svg";
    string fileTittle = "C:/Users/deyne/Desktop/asd/images/svg/" + newFileName;
    tinyxml2::XMLElement* groupPaths = doc->NewElement("g");
    
    while(!finishGeneratePath || !this->queuePaths.empty()) {
        if (!this->queuePaths.empty()) {
            groupPaths->InsertEndChild(this->queuePaths.front());
            this->queuePaths.pop();
        }
    }
    doc->FirstChildElement()->InsertEndChild(groupPaths);
    doc->SaveFile(fileTittle.c_str());
    groupPaths->DeleteChildren();
}

