class Observer {
public:
    ~Observer();
    virtual void update() = 0;
};

class Subject {
public:
    ~Subject();
    virtual void notify() = 0;
};
