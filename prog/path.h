#ifndef PATH_H
#define PATH_H

#include <iostream>
#include <vector>
using namespace std;

class Path {
private:

    string typePath;
    vector<float> areaPoints;
    vector<float> pathsX;
    vector<float> pathsY;
    string color;

    int amountFrames;
    int count;

public:

    Path(vector<float> pPathsX, vector<float> pPathsY, vector<float> pAreaPoints, string pColor, string pTypePath){
        this->pathsX = pPathsX;
        this->pathsY = pPathsY;
        this->areaPoints = pAreaPoints;
        this->typePath = pTypePath;
        this->color = pColor;
        this->amountFrames = 0;
        count = 1;
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
    string getTypePaths() {
        return typePath;
    }
    string getColor() {
        return color;
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