#include "StudentWorld.h"
#include <string>
#include <vector>
#include <memory>
using namespace std;

StudentWorld::StudentWorld(string assetDir): GameWorld(assetDir){}

StudentWorld::~StudentWorld(){}

//skeleton;
int StudentWorld::init(){
    //tunnelman object getting placed in right spot
    Tunnelman* tMan = new Tunnelman();
    Earth* earthObjects[64][64];
    //Earth earth(int,int);
    //unique_ptr<Actor> earthPtr;
    //each bucket in the 2d array has a unique ptr that points to a vector of unique pointers that point to a game object
//    vector<unique_ptr<Actor>> V;
//    unique_ptr<vector<unique_ptr<Actor>>> arrOfVects[64][64];
    //push a pointer to an eart object into the vector in each array bucket
    //left of mine shaft in mid rows:0-29 cols:0-59
    for (int c = 0; c < 29; c++){
        for (int r = 0; r< 59; r++){
            //unique_ptr<Earth> ePtr = make_unique<Earth>Earth(r, c)
            earthObjects[c][r] = new Earth(c,r);
        }
    }
    //right of mine shaft
    for (int c = 34; c < 64; c++){
        for (int r = 0; r< 59; r++){
            //unique_ptr<Earth> ePtr = make_unique<Earth>Earth(r, c)
            earthObjects[c][r] = new Earth(c,r);
        }
    }
    
    //earth at the end of mine shaft
    for (int c = 29; c < 34; c++){
        for (int r = 0; r< 4; r++){
            //unique_ptr<Earth> ePtr = make_unique<Earth>Earth(r, c)
            earthObjects[c][r] = new Earth(c,r);
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move(){
    return GWSTATUS_CONTINUE_GAME;
}

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
