#include "Router.h"

void Router::update() {
    calculate_route();
}
void Router::attach(Observer* obs) {
    this->generator = obs;
}
void Router::notify() {
    this->generator->update();
}

Router::Router(tinyxml2::XMLDocument &pDoc, vector<vector<float>> pValuesPaths, int pFrames, float pAngle, vector<vector<float>> *pPositionPaths) {
    this->doc = &pDoc;
    this->valuesPaths = pValuesPaths;
    this->frames = pFrames;
    this->angle = pAngle;
    this->positionPaths = pPositionPaths;
    sizeBox(pDoc, sizeX, sizeY);
}

void Router::sizeBox(tinyxml2::XMLDocument &pDoc, string &pSizeX, string &pSizeY) {
    tinyxml2::XMLElement *pRoot = pDoc.FirstChildElement("svg");
    string view = pRoot->FindAttribute("viewBox")->Value();
    view.erase(0, view.find(' ') + 1);
    view.erase(0, view.find(' ') + 1);
    pSizeX = view.substr(0, view.find(' '));
    view.erase(0, view.find(' ') + 1);
    pSizeY = view.substr(0, view.find(' '));
}

void Router::calculate_route() {
    float vX = this->frames;
    float vY = 0;
    this->angle = (this->angle * 3.1415) / 180.0;

    float new_vX = (vX * cos(this->angle)) + (vY * sin(this->angle));
    float new_vY = -(vX * sin(this->angle)) + (vY * cos(this->angle));

    vX = new_vX;
    vY = new_vY;
    int selector=0;
    int cont_Frames=0;

    while (cont_Frames < this->frames) {
        vector<float> vectorX = valuesPaths[selector];
        vector<float> vectorY = valuesPaths[selector + 1];

        bool continuar = true;
        for (int j = 0; j < vectorX.size(); j++)
        {
            if (vectorX[j] > stoi(sizeX))
            {
                continuar = false;
                break;
            }
            vectorX[j] += vX;
        }
        for (int j = 0; j < vectorY.size() && continuar == true; j++)
        {
            if (vectorY[j] > stoi(sizeY))
            {
                break;
            }
            vectorY[j] += vY;
        }

        valuesPaths[selector] = vectorX;
        valuesPaths[selector + 1] = vectorY;

        selector += 2;
        this->positionPaths->push_back(vectorX);
        this->positionPaths->push_back(vectorY);
        
        if(selector >= valuesPaths.size()){
            notify();
            selector = 0;
            cont_Frames++;

            this->positionPaths->clear();
        }
    }
}