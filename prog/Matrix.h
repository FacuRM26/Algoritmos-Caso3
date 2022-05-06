#ifndef MATRIX_H
#define MATRIX_H
#include <stdexcept>
#include <iostream>
#include <vector>
using namespace std;
using std::runtime_error;
template <typename E>


class  Matrix
{
private:
    int rows;
    int columns;
    E **matrix;

public:
    Matrix(int rows, int columns){
        if(rows<=0 || columns <=0)
            throw runtime_error("Number of rows and columns must greater than zero");
            this ->rows=rows;
            this ->columns=columns;
            matrix = new E*[rows];
            for (int i=0; i<rows; i++){
                matrix[i] = new E[columns];
            }}
    ~Matrix(){
    for(int i=0; i<rows;i++){
        delete[]matrix[i];
    }
    delete []matrix;
    }
E getvalue(int row,int column){
    if (row <0 || row>=rows){
        throw runtime_error("Invalid row");
    }
    if (column <0 || column>=columns){
        throw runtime_error("Invalid column");
    }
    return matrix[row][column];
}

void setvalue(int row, int column, E value){
    if (row <0 || row>=rows){
        throw runtime_error("Invalid row");}
    if (column <0 || column>=columns){
        throw runtime_error("Invalid column");}
    matrix[row][column]= value;
}

int getrows(){
    return rows;}
int getcolumns(){
    return columns;}

void setAll(E value){
	for (int i=0; i<rows; i++){
		for (int j=0; j<columns; j++){
			matrix[i][j]=value;
		}}}

void transpose(){
    E **temp;
    int num;

    temp = new E*[columns];
            for (int i=0; i<columns; i++){
                temp[i] = new E[rows];
}
    for (int i=0;i<rows;i++){
            for (int j=0; j<columns; j++){
                temp[j][i]=matrix[i][j];
    }
}
    for(int i=0; i<rows;i++){
        delete[]matrix[i];
        }
    delete []matrix;
    num=rows;
    rows=columns;
    columns=num;
    matrix=temp;

}
void addRow(E value){
    E **temp;
    int num=rows;
    rows++;
    temp = new E*[rows];
    for (int i=0; i<num; i++){
        temp[i] = matrix[i];
    }
    temp[num]=new E[columns];
    for (int i=0;i<columns;i++){
        temp[num][i] = value;
    }
    for(int i=0; i<num;i++){
        delete[]matrix[i];
        }
    delete []matrix;
    matrix=temp;
}

void addColumn(E value){
    E **temp;
    int num=columns;
    columns++;
    temp = new E*[rows];
    for (int i=0; i<rows; i++){
        temp[i] = new E[columns];
        temp[i][num]=value;
}
    for(int i=0; i< rows;i++){
        for (int j=0;j<num;j++){
            temp[i][j]= matrix[i][j];
        }
    }

    for(int i=0; i<rows;i++){
        delete[]matrix[i];
    }
    delete []matrix;
    matrix=temp;
}

void removeRow(int row){
    if (row<0 ||row>rows){
       throw runtime_error("Invalid row");}
    E **temp;
    int t=0;
    int m=0;
    rows--;
    temp = new E*[rows];
    for (int i=0; i<rows; i++){
        temp[i] = new E[columns];
    }
    while(m<=rows){
        if (m != row){
            temp[t] = matrix[m];
            t++;
        }
        m+=1;
    }

    for(int i=0; i<m;i++){
        delete[]matrix[i];
    }
    delete []matrix;
    matrix=temp;

}

void removeColumn(int column){
    if (column<0 ||column>columns){
       throw runtime_error("Invalid column");
       }

    E **temp;
    int t=0;
    columns--;
    temp = new E*[rows];
    for (int i=0; i<rows; i++){
        temp[i] = new E[columns];
    }

    for (int i=0;i<rows;i++){
        t=0;

        for (int j=0;j<(columns+1);j++){
            //cout<<t<<endl;
            if(j!=column){
                temp[i][t]=matrix[i][j];
                t++;
            }
        }
    }
    for(int i=0; i<(columns+1);i++){
        delete[]matrix[i];
    }
    delete []matrix;
    matrix=temp;
}
void print(){
    for(int i=0; i< rows;i++){
        for (int j=0;j<columns;j++){
            cout<< matrix[i][j]<<"\t";
        }
        cout<<endl;
    }
   cout<<endl;
}
bool empty(int row, int column){
    if (matrix[row][column] == '\0') {
      printf("El elemento %d de la matriz es Null\n");
      return true;
    }
    return false;

}
};

#endif
