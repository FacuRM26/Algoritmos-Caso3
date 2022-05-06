#ifndef SUBJECT_OBSERVER_H
#define SUBJECT_OBSERVER_H

#include "path.h"

#include <vector>
using namespace std;

class Observer {
public:
    virtual void update(vector<Path*> pathsIntersected) = 0;
};

class Subject {
public:
    virtual void attach(Observer* observer) = 0;
    virtual void notify() = 0;
};

#endif