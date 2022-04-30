
#ifndef SELECCION_H
#define SELECCION_H

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include "../tinyXML/tinyxml2.cpp"
#include "ObserverSubject.h"
using namespace std;

class seleccion : public Subject {
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

    void getMoveValues(string moveSection, float *Move_x, float *Move_y);
    void getCurveValues(string curveSection, vector<float> curvePoints[], float Move_x, float Move_y, string* types);
    void getAllPaths(tinyxml2::XMLElement* element);
    void saveRGBColors(int userColors[], int size);
    bool checkColorIntersection(int color);
    bool pathIntersect(vector<float> curvePoints[], float* majorX, float* minorX, float* majorY, float* minorY);

public:
    seleccion(int userPoints[], int size_userPoints, int userColors[], int size_userColors, tinyxml2::XMLDocument &doc);
    void selectPaths();

    void attach(Observer* router);
    void notify();

    vector<string> getTypePath();
    vector<float> getPathsArea();
    vector<vector<float>> getPathValues();
};

#endif