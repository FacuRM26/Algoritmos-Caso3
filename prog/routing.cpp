#include "Routing.h"

Routing::Routing(tinyxml2::XMLDocument &doc, vector<vector<float>> valuesPaths)
{
    this->doc = &doc;
    this->valuesPaths = valuesPaths;
    sizeBox(doc, sizeX, sizeY);
}

void Routing::sizeBox(tinyxml2::XMLDocument &doc, string &sizeX, string &sizeY)
{
    tinyxml2::XMLElement *pRoot = doc.FirstChildElement("svg");
    string view = pRoot->FindAttribute("viewBox")->Value();
    view.erase(0, view.find(' ') + 1);
    view.erase(0, view.find(' ') + 1);
    sizeX = view.substr(0, view.find(' '));
    view.erase(0, view.find(' ') + 1);
    sizeY = view.substr(0, view.find(' '));
}

vector<vector<float>> Routing::calculate_route(int frames, float angulo)
{
    vector<vector<float>> coordenadas;
    float vX = vX + frames;
    float vY = 0;
    angulo = (angulo * 3.1415) / 180.0;

    float new_vX = (vX * cos(angulo)) + (vY * sin(angulo));
    float new_vY = -(vX * sin(angulo)) + (vY * cos(angulo));

    vX = new_vX;
    vY = new_vY;
    int selector=0;
    int cont_Frames=0;
    while (cont_Frames<frames)
    {
        vector<float> vectorX = valuesPaths[selector];
        vector<float> vectorY = valuesPaths[selector+ 1];
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
        selector++;
        if(selector>=valuesPaths.size()){
            //generador
            selector=0;
            cont_Frames++;
        }
    }
    return coordenadas;
}
