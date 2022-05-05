#ifndef SUBJECT_OBSERVER_H
#define SUBJECT_OBSERVER_H

#include <vector>
using namespace std;

class Observer {
public:
    virtual void update(vector<vector<float>> positionPaths) = 0;
};

class Subject {
public:
    virtual void attach(Observer* observer) = 0;
    virtual void notify() = 0;
};

#endif