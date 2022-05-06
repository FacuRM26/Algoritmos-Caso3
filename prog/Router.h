#ifndef ROUTER_H
#define ROUTER_H


#include <string>
#include <vector>
#include <iostream>
#include <list>
#include <cmath>
#include <algorithm>
#include <utility>
#include "path.h"

using namespace std;

class Router : public Observer, public Subject {
private:
    string sizeX;
    string sizeY;

    Observer* generator = 0;

    vector<vector<float>>* positionPaths;
    vector<vector<float>> valuesPaths;
    tinyxml2::XMLDocument* doc;
    float angle;
    int frames;
    int calculateDirection();
    Path calculateDistance(vector<float>pVectorX,vector<float>pVectorY,float vX,float vY,int pNum);
    void sizeBox(tinyxml2::XMLDocument &pDoc, string &pSizeX, string &pSizeY);

public:
    Router(tinyxml2::XMLDocument &pDoc, vector<vector<float>> pValuesPaths, int pFrames, float pAngle, vector<vector<float>> *pPositionPaths);
    void calculate_route();

    void update();
    void attach(Observer* obs);
    void notify();

};
#endif // Router_H