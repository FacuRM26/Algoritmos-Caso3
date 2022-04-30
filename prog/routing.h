#ifndef ROUTING_H
#define ROUTING_H

#include <string>
#include <vector>
#include <iostream>
#include <list>
#include <cmath>
#include <algorithm>
#include <utility>

using namespace std;

class Routing : public Observer, public Subject {
private:
    string sizeX;
    string sizeY;

    Observer* generator = 0;

    vector<vector<float>>* positionPaths;
    vector<vector<float>> valuesPaths;
    tinyxml2::XMLDocument* doc;
    float angulo;
    int frames;
    
    void sizeBox(tinyxml2::XMLDocument &doc, string &sizeX, string &sizeY);

public:
    Routing(tinyxml2::XMLDocument &doc, vector<vector<float>> valuesPaths, int frames, float angulo, vector<vector<float>> *positionPaths);
    void calculate_route();

    void update();
    void attach(Observer* obs);
    void notify();

};
#endif // ROUTING_H