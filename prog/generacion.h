
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

    vector<Path*> pathsIntersected;

    string generateAttribute(int* x_Selector, int* y_Selector, vector<float> valuesX, vector<float> valuesY, char typePath);

public:
    generacion(tinyxml2::XMLDocument &doc);

    void update(vector<Path*> pPathsIntersected);
    void generateFile();
};



#endif
