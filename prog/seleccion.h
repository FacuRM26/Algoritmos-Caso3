
#ifndef SELECCION_H
#define SELECCION_H

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include "../tinyXML/tinyxml2.cpp"
using namespace std;

class seleccion {
private:
    int* userPoints;
    int size_userPoints;
    tinyxml2::XMLDocument* doc;
    vector<tinyxml2::XMLElement*> paths;
    vector<tinyxml2::XMLElement*> pathsIntersected;
    vector<vector<int>> userColors;

    void getMoveValues(string moveSection, float *Move_x, float *Move_y);
    void getCurveValues(string curveSection, vector<float> curvePoints[], float Move_x, float Move_y);
    void getAllPaths(tinyxml2::XMLElement* element);
    void saveRGBColors(int userColors[], int size);
    bool checkColorIntersection(int color);
    bool pathIntersect(vector<float> curvePoints[]);

public:
    seleccion(int userPoints[], int size_userPoints, int userColors[], int size_userColors, tinyxml2::XMLDocument &doc);
    void selectPaths();
};

#endif