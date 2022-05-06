
#ifndef GENERATOR_H
#define GENERATOR_H

#include <iostream>
#include "ObserverSubject.h"
using namespace std;

class Generator : public Observer {
private:
    int fileCounter = 1;
    tinyxml2::XMLDocument* doc;
    vector<string> typePaths;

    vector<vector<float>>* positionPaths;

public:
    Generator(tinyxml2::XMLDocument &pDoc, vector<string> pTypePaths, vector<vector<float>> *pPositionPaths);

    void update();
    void generateFile();
};



#endif
