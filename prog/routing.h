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
    vector<Path*> pathsIntersected;

    Observer* generator = 0;

    tinyxml2::XMLDocument* doc;
    float angle;
    int frames;
    
    void sizeBox(tinyxml2::XMLDocument &doc, string &sizeX, string &sizeY);
    void calculateDistance(Path* path, float vX, float vY, int pNum);
    int calculateDirection();

public:
    Routing(tinyxml2::XMLDocument &doc, int frames, float angulo);
    void calculate_route();

    void update(vector<Path*> pPathsIntersected);
    void attach(Observer* obs);
    void notify();

};
#endif // ROUTING_H