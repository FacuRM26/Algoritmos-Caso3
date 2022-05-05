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

void Router::calculate_route()
{
    float vX = this->frames;
    float vY = 0;
    this->angle = (this->angle * 3.1415) / 180.0;

    float new_vX = (vX * cos(this->angle)) + (vY * sin(this->angle));
    float new_vY = -(vX * sin(this->angle)) + (vY * cos(this->angle));

    vX = new_vX;
    vY = new_vY;
    int selector=0;
    vector<Path> paths;
    // cout << "SizeY: " << sizeY << endl;
    // cout << "vY" << vY << endl;
    
    // cout<<"Size: "<<valuesPaths.size()<<endl;
    while (!(selector >= valuesPaths.size()))
    {   

        vector<float> vectorX = valuesPaths[selector];
        vector<float> vectorY = valuesPaths[selector + 1];

        float distanceX = frames / abs(((stoi(sizeX) - vectorX[vectorX.size()-1]) / vX));
        float distanceY = frames / abs(((stoi(sizeY) - vectorY[vectorY.size()-1]) / vY));
        Path newPath(vectorX, vectorY);

        if (distanceX > distanceY){
            newPath.setFrames(round(distanceX));
            //cout<<"distanceX: "<<distanceX<<endl;
            }
        else{
            //cout<<"distanceY: "<<distanceY<<endl;
            //cout<<"distanceY:"<<stoi(sizeY) - vectorY[0]<<endl;
            newPath.setFrames(round(distanceY));
        }
        paths.push_back(newPath);
        selector+=2;

    }
    //cout<<"salio"<<endl;
    
    int cont_Frames = 0;
    int listSelector = 0;
    //cout<<paths.size()<<endl;
    //cout<<paths[0].getAmount()<<endl;
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
        //cont_Frames++;
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