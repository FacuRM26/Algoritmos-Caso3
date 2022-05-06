#ifndef PATH_H
#define PATH_H

#include <iostream>
#include <vector>
using namespace std;

class Path {
private:

    vector<float> pathsX;
    vector<float> pathsY;
    int amountFrames;
    int count;

public:

    Path(vector<float> pPathsX,vector<float> pPathsY){
        this->pathsX=pPathsX;
        this->pathsY=pPathsY;
        this->amountFrames=0;
        count=1;
    }
    void setFrames(int pFrames){
        if(pFrames<1){
            pFrames=1;
            this->count=pFrames;
            return;
        }
        this->count=pFrames;
        this->amountFrames=pFrames;
        return;
    }
    vector<float> getPathsX(){
        return pathsX;
    }
    vector<float> getPathsY(){
        return pathsY;
    }
    bool movePath(){
        if (count<amountFrames){
            count++;
            return false;
        }
        count=1;
        return true;
    }
    void setVectors(vector<float> pPathsX,vector<float> pPathsY){
        this->pathsX=pPathsX;
        this->pathsY=pPathsY;
    }
    int getAmount(){
        return amountFrames;
    }
};



#endif