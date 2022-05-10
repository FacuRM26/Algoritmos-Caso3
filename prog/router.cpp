#include "router.h"

void Router::update(vector<Path*> pPathsIntersected)
{
    cout << "\n > Iniciando proceso de routing y generacion...";
    this->pathsIntersected = pPathsIntersected;
    calculate_route();
    cout << "\n > Proceso de routing y generacion finalizado\n";
}
void Router::attach(Observer *obs)
{
    this->generator = obs;
}
void Router::notify() {
    this->generator->update(this->pathsIntersected);
}

Router::Router(tinyxml2::XMLDocument &pDoc, int pFrames, float pAngle)
{
    this->doc = &pDoc;
    this->frames = pFrames;
    this->angle = pAngle;
    sizeBox(pDoc, sizeX, sizeY);
}

void Router::sizeBox(tinyxml2::XMLDocument &pDoc, string &pSizeX, string &pSizeY)
{
    tinyxml2::XMLElement *pRoot = pDoc.FirstChildElement("svg");
    string view = pRoot->FindAttribute("viewBox")->Value();
    view.erase(0, view.find(' ') + 1);
    view.erase(0, view.find(' ') + 1);
    pSizeX = view.substr(0, view.find(' '));
    view.erase(0, view.find(' ') + 1);
    pSizeY = view.substr(0, view.find(' '));
}
int Router::calculateDirection(){
    float pi = 2*acos(0);
    
    if (angle <= (pi/2))
        return 1;
    else if((angle > (pi/2)) && (angle <= pi))
        return 2;
    else if((angle > pi) && (angle <= ((3*pi)/2)))
        return 3;
    else if((angle > ((3*pi)/2)) && (angle <= (2*pi)))
        return 4;
    return 0;
}
void Router::calculateDistance(Path* pPath, float pDirectionX, float pDirectionY, int pNum) {

    float distanceX;
    float distanceY;

    vector<float> pVectorX = pPath->getPathsX();
    vector<float> pVectorY = pPath->getPathsY();

    if (pNum == 1){
        distanceX = frames / abs(((stoi(sizeX) - pVectorX[pVectorX.size() - 1]) / pDirectionX));
        distanceY = frames / abs(((pVectorY[pVectorY.size() - 1]) / pDirectionY));   
    } else if(pNum == 2) {
        distanceX = frames / abs(((pVectorX[pVectorX.size() - 1]) / pDirectionX));
        distanceY = frames / abs(((pVectorY[pVectorY.size() - 1]) / pDirectionY));   
    } else if(pNum == 3) {
        distanceX = frames / abs(((pVectorX[pVectorX.size() - 1]) / pDirectionX));
        distanceY = frames / abs(((stoi(sizeY) - pVectorY[pVectorY.size() - 1]) / pDirectionY));   
    } else if(pNum == 4) {
        distanceX = frames / abs(((stoi(sizeX) - pVectorX[pVectorX.size() - 1]) / pDirectionX));
        distanceY = frames / abs(((stoi(sizeY) - pVectorY[pVectorY.size() - 1]) / pDirectionY));
    }
    if (distanceX > distanceY)
        pPath->setFrames(round(distanceX));                           
    else
        pPath->setFrames(round(distanceY));
}
void Router::calculate_route()
{
    float directionX = this->frames;
    float directionY = 0;

    float new_directionX = (directionX * cos(this->angle)) + (directionY * sin(this->angle));
    float new_directionY = -(directionX * sin(this->angle)) + (directionY * cos(this->angle));

    directionX = new_directionX;
    directionY = new_directionY;

    int num = calculateDirection();
    for (auto path : pathsIntersected)
        calculateDistance(path, directionX, directionY, num);

    int count_Frames = 0;
    int listSelector = 0;

    while (count_Frames < this->frames) {
        vector<float> vectorX = pathsIntersected[listSelector]->getPathsX();
        vector<float> vectorY = pathsIntersected[listSelector]->getPathsY();

        if (pathsIntersected[listSelector]->movePath()) {
            for (int coordinateX = 0; coordinateX < vectorX.size(); coordinateX++)
                vectorX[coordinateX] += directionX;
            for (int coordinateY = 0; coordinateY < vectorY.size(); coordinateY++)
                vectorY[coordinateY] += directionY;
            pathsIntersected[listSelector]->setVectors(vectorX, vectorY);
        }

        listSelector++;

        if (listSelector >= pathsIntersected.size()) {
            notify();
            listSelector = 0;
            count_Frames++;
        }
    }
    return;
}