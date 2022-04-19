
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

    void getMoveValues(string moveSection, float *Move_x, float *Move_y);
    void getCurveValues(string curveSection, vector<float> curvePoints[], float Move_x, float Move_y);
    bool pathIntersect(vector<float> curvePoints[]);

public:
    seleccion(int userPoints[], int size_userPoints, tinyxml2::XMLDocument &doc);
    void selectPaths();
};

#endif