#include "Router.h"

void Router::update()
{
    calculate_route();
}
void Router::attach(Observer *obs)
{
    this->generator = obs;
}
// tipo
void Router::notify()
{
    this->generator->update();
}

Router::Router(tinyxml2::XMLDocument &pDoc, vector<vector<float>> pValuesPaths, int pFrames, float pAngle, vector<vector<float>> *pPositionPaths)
{
    this->doc = &pDoc;
    this->valuesPaths = pValuesPaths;
    this->frames = pFrames;
    this->angle = pAngle;
    this->positionPaths = pPositionPaths;
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
Path Router::calculateDistance(vector<float> pVectorX, vector<float> pVectorY,float vX,float vY,int pNum)
{

    float distanceX;
    float distanceY;
    Path newPath(pVectorX, pVectorY);
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
        newPath.setFrames(round(distanceX));                           
    }
    else
    {
        newPath.setFrames(round(distanceY));
    }
    return newPath;
}
void Router::calculate_route()
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
    int num=calculateDirection();
    while (!(selector >= valuesPaths.size()))
    {
        vector<float> vectorX = valuesPaths[selector];
        vector<float> vectorY = valuesPaths[selector + 1];
        Path newPath=calculateDistance(vectorX,vectorY,vX,vY,num);
        paths.push_back(newPath);
        selector += 2;
    }
    cout<<"salio"<<endl;

    int cont_Frames = 0;
    int listSelector = 0;
    // cout<<paths.size()<<endl;
    // cout<<paths[0].getAmount()<<endl;
    while (cont_Frames < this->frames)
    {
        // cout<<cont_Frames<<endl;
        // cout<<"listselector: "<<listSelector<<endl;
        vector<float> vectorX = paths[listSelector].getPathsX();
        vector<float> vectorY = paths[listSelector].getPathsY();

        if (paths[listSelector].movePath())
        {

            for (int j = 0; j < vectorX.size(); j++)
            {
                vectorX[j] += vX;
            }
            for (int j = 0; j < vectorY.size(); j++)
            {
                vectorY[j] += vY;
            }
            paths[listSelector].setVectors(vectorX, vectorY);
        }

        listSelector++;
        this->positionPaths->push_back(vectorX);
        this->positionPaths->push_back(vectorY);
        // cont_Frames++;
        if (listSelector >= paths.size())
        {
            notify();
            listSelector = 0;
            cont_Frames++;
            this->positionPaths->clear();
        }
    }
    return;
}