#include "Routing.h"

void Routing::update(vector<vector<float>> positionPaths) {
    calculate_route();
}
void Routing::attach(Observer* obs) {
    this->generator = obs;
}
void Routing::notify() {
    this->generator->update(this->positionPaths);
}

Routing::Routing(tinyxml2::XMLDocument &doc, vector<vector<float>> valuesPaths, int frames, float angulo) {
    this->doc = &doc;
    this->valuesPaths = valuesPaths;
    this->frames = frames;
    this->angulo = angulo;
    this->positionPaths = {};
    sizeBox(doc, sizeX, sizeY);
}

void Routing::sizeBox(tinyxml2::XMLDocument &doc, string &sizeX, string &sizeY) {
    tinyxml2::XMLElement *pRoot = doc.FirstChildElement("svg");
    string view = pRoot->FindAttribute("viewBox")->Value();
    view.erase(0, view.find(' ') + 1);
    view.erase(0, view.find(' ') + 1);
    sizeX = view.substr(0, view.find(' '));
    view.erase(0, view.find(' ') + 1);
    sizeY = view.substr(0, view.find(' '));
}

void Routing::calculate_route() {
    float vX = this->frames;
    float vY = 0;
    this->angulo = (this->angulo * 3.1415) / 180.0;

    float new_vX = (vX * cos(this->angulo)) + (vY * sin(this->angulo));
    float new_vY = -(vX * sin(this->angulo)) + (vY * cos(this->angulo));

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
        this->positionPaths.push_back(vectorX);
        this->positionPaths.push_back(vectorY);
        
        if(selector >= valuesPaths.size()){
            notify();
            selector = 0;
            cont_Frames++;

            this->positionPaths.clear();
        }
    }
}