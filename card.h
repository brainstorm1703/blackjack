#ifndef CARD_H
#define CARD_H
#include <iostream>

using namespace std;

struct card
{
public:

    card (int id, int value, string pathToFile) : id(id), value(value), pathToFile(pathToFile){};

    int getId(){
       return id;
    }

    void setId(int id){
       this->id = id;
    }

    int getValue(){
       return value;
    }

    void setValue(int value){
       this->value = value;
    }

    string getPathToFile(){
       return pathToFile;
    }

    void setPathToFile(string pathToFile){
       this->pathToFile = pathToFile;
    }

private:
    int id;
    int value;
    string pathToFile;


};


#endif // CARD_H
