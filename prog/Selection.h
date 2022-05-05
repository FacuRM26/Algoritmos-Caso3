
#ifndef SELECTION_H
#define SELECTION_H

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include "../tinyXML/tinyxml2.cpp"
#include "ObserverSubject.h"

using namespace std;

class Selection : public Subject {
private:
    int* userPoints;
    int size_userPoints;
    tinyxml2::XMLDocument* doc;
    vector<tinyxml2::XMLElement*> paths;
    vector<vector<int>> userColors;

    vector<string> typePath;
    vector<float> pathsArea;
    vector<vector<float>> pathValues;

    Observer* current_router = 0;

    void getMoveValues(string pMoveSection, float *pMove_x, float *pMove_y);
    void getCurveValues(string pCurveSection, vector<float> pCurvePoints[], float pMove_x, float pMove_y, string* pTypes);
    void getAllPaths(tinyxml2::XMLElement* pElement);
    void saveRGBColors(int pUserColors[], int pSize);
    bool checkColorIntersection(int pColor);
    bool pathIntersect(vector<float> pCurvePoints[], float* pMajorX, float* pMinorX, float* pMajorY, float* pMinorY);

public:
    Selection(int userPoints[], int size_userPoints, int userColors[], int size_userColors, tinyxml2::XMLDocument &doc);
    void selectPaths();

    void attach(Observer* pRouter);
    void notify();

    vector<string> getTypePath();
    vector<float> getPathsArea();
    vector<vector<float>> getPathValues();
};

#endif