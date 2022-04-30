
#ifndef GENERACION_H
#define GENERACION_H

#include <iostream>
#include "ObserverSubject.h"
using namespace std;

class generacion : public Observer {
private:
    int fileCounter = 1;
    tinyxml2::XMLDocument* doc;
    vector<string> typePaths;

    vector<vector<float>>* positionPaths;

public:
    generacion(tinyxml2::XMLDocument &doc, vector<string> typePaths, vector<vector<float>> *positionPaths);

    void update();
    void generateFile();
};



#endif
