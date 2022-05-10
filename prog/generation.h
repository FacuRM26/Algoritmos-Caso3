#ifndef GENERATION_H
#define GENERATION_H

#include <iostream>
#include <thread>
#include <unistd.h>
#include <queue>
#include "ObserverSubject.h"
using namespace std;

class Generation : public Observer {
private:
    int fileCounter = 1;
    tinyxml2::XMLDocument* doc;
    vector<Path*> pathsIntersected;
    queue<tinyxml2::XMLElement*> queuePaths;
    string fileName;
    volatile bool finishGeneratePath;

    string generateAttribute(int* pX_Selector, int* pY_Selector, vector<float> pValuesX, vector<float> pValuesY, char pTypePath);
    void lessAppearances(int* pIndex, int* pAppearences, string pType);
    void generatePath();
    void generateFile();


public:
    Generation(tinyxml2::XMLDocument &pDoc, string fileName);
    void update(vector<Path*> pPathsIntersected);
};

#endif
