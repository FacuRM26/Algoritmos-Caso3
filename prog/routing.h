#ifndef ROUTING_H
#define ROUTING_H

#include "Matrix.h"
#include <string>
#include <vector>
#include <iostream>
#include <list>
#include <cmath>
#include <algorithm>
#include <utility>

using namespace std;

class Routing
{
private:
    string sizeX;
    string sizeY;
    tinyxml2::XMLDocument* doc;

    vector<vector<float>> valuesPaths;

    void sizeBox(tinyxml2::XMLDocument &doc, string &sizeX, string &sizeY);

public:
    Routing(tinyxml2::XMLDocument &doc, vector<vector<float>> valuesPaths);
    vector<vector<float>>  calculate_route(int frames, float angulo);

};
#endif // ROUTING_H