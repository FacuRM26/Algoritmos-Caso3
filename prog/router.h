#ifndef ROUTER_H
#define ROUTER_H

#include <vector>
#include <iostream>
#include <cmath>
#include <utility>

using namespace std;

class Router : public Observer, public Subject {
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
    void calculate_route();
    int calculateDirection();

public:
    Router(tinyxml2::XMLDocument &doc, int frames, float angulo);

    void update(vector<Path*> pPathsIntersected);
    void attach(Observer* obs);
    void notify();

};
#endif // ROUTING_H