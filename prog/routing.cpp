#include "routing.h"

void Routing::update(vector<Path*> pPathsIntersected)
{
    cout << "\nIniciando proceso de routing...\n";
    this->pathsIntersected = pPathsIntersected;
    calculate_route();
}
void Routing::attach(Observer *obs)
{
    this->generator = obs;
}
// tipo
void Routing::notify() {
    cout << "\nIniciando proceso de generacion...\n";
    this->generator->update(this->pathsIntersected);
    cout << "\nGeneracion terminada, volviendo al routing...\n";
}

Routing::Routing(tinyxml2::XMLDocument &pDoc, int pFrames, float pAngle)
{
    this->doc = &pDoc;
    this->frames = pFrames;
    this->angle = pAngle;
    sizeBox(pDoc, sizeX, sizeY);
}

void Routing::sizeBox(tinyxml2::XMLDocument &pDoc, string &pSizeX, string &pSizeY)
{
    tinyxml2::XMLElement *pRoot = pDoc.FirstChildElement("svg");
    string view = pRoot->FindAttribute("viewBox")->Value();
    view.erase(0, view.find(' ') + 1);
    view.erase(0, view.find(' ') + 1);
    pSizeX = view.substr(0, view.find(' '));
    view.erase(0, view.find(' ') + 1);
    pSizeY = view.substr(0, view.find(' '));
}
int Routing::calculateDirection(){
    float pi=2*acos(0);
    if (angle<=(pi/2)){
        return 1;
    }else if(angle>(pi/2)&&angle<=pi){
        return 2;
    }else if(angle>pi && angle<=((3*pi)/2)){
        return 3;
    }else if(angle>((3*pi)/2)&&angle<=2*pi){
        return 4;
    }
    return 0;
}
void Routing::calculateDistance(Path* path, float vX, float vY, int pNum) {

    float distanceX;
    float distanceY;

    vector<float> pVectorX = path->getPathsX();
    vector<float> pVectorY = path->getPathsY();

    if (pNum=1){
        distanceX = frames / abs(((stoi(sizeX) - pVectorX[pVectorX.size() - 1]) / vX));
        distanceY = frames / abs(((pVectorY[pVectorY.size() - 1])/ vY));   
    }else if(pNum=2){
        distanceX = frames / abs(((pVectorX[pVectorX.size() - 1]) / vX));
        distanceY = frames / abs(((pVectorY[pVectorY.size() - 1]) / vY));   
    }else if(pNum=3){
        distanceX = frames / abs(((pVectorX[pVectorX.size() - 1]) / vX));
        distanceY = frames / abs(((stoi(sizeY) - pVectorY[pVectorY.size() - 1]) / vY));   

    }else if(pNum=4){
        distanceX = frames / abs(((stoi(sizeX) - pVectorX[pVectorX.size() - 1]) / vX));
        distanceY = frames / abs(((stoi(sizeY) - pVectorY[pVectorY.size() - 1]) / vY));
    }
    if (distanceX > distanceY)
    {
        path->setFrames(round(distanceX));                           
    }
    else
    {
        path->setFrames(round(distanceY));
    }
}
void Routing::calculate_route()
{
    float vX = this->frames;
    float vY = 0;
    this->angle = (this->angle * 3.1415) / 180.0;

    float new_vX = (vX * cos(this->angle)) + (vY * sin(this->angle));
    float new_vY = -(vX * sin(this->angle)) + (vY * cos(this->angle));

    vX = new_vX;
    vY = new_vY;
    int selector = 0;
    vector<Path> paths;

    int num = calculateDirection();
    for (auto path : pathsIntersected) {
        calculateDistance(path, vX, vY, num);
    }

    int cont_Frames = 0;
    int listSelector = 0;

    while (cont_Frames < this->frames) {
        vector<float> vectorX = pathsIntersected[listSelector]->getPathsX();
        vector<float> vectorY = pathsIntersected[listSelector]->getPathsY();

        if (pathsIntersected[listSelector]->movePath())
        {
            for (int j = 0; j < vectorX.size(); j++)
            {
                vectorX[j] += vX;
            }
            for (int j = 0; j < vectorY.size(); j++)
            {
                vectorY[j] += vY;
            }
            pathsIntersected[listSelector]->setVectors(vectorX, vectorY);
        }

        listSelector++;

        if (listSelector >= pathsIntersected.size()) {
            notify();
            listSelector = 0;
            cont_Frames++;
        }
    }
    return;
}