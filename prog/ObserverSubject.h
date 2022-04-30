#ifndef SUBJECT_OBSERVER_H
#define SUBJECT_OBSERVER_H

class Observer {
public:
    virtual void update() = 0;
};

class Subject {
public:
    virtual void attach(Observer* observer) = 0;
    virtual void notify() = 0;
};

#endif