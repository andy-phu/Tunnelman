#include "StudentWorld.h"
#include <string>
#include <vector>
#include <memory>
using namespace std;

StudentWorld::StudentWorld(string assetDir): GameWorld(assetDir){
}

StudentWorld::~StudentWorld(){
    ;
}

//skeleton;
int StudentWorld::init(){
    //tunnelman object getting placed in right spot
    Tunnelman tMan;
    //Earth earth(int,int);
    //unique_ptr<Actor> earthPtr;
    //each bucket in the 2d array has a unique ptr that points to a vector of unique pointers that point to a game object

    Earth* earthObjects[64][64];

    //push a pointer to an eart object into the vector in each array bucket
    //left of mine shaft in mid rows:0-59 cols:0-29
    for (int r = 0; r < 59; r++){
        for (int c = 0; c < 30; c++){

            earthObjects[r][c] = new Earth(r, c);

        }
    }
    //right of mine shaft

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move(){
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {

}

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
