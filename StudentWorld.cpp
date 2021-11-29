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
    //put tunnelman in a grid
//    vector<Tunnelman*> tunnelV[64][64];
//    for (int c = 0; c < 64; c++){
//        for (int r = 0; r< 64; r++){
//            //unique_ptr<Earth> ePtr = make_unique<Earth>Earth(r, c)
//            tunnelV[c][r].push_back(nullptr);
//            if(c == 30 && r==60){
//                tunnelV[c][r].push_back(tMan);
//            }
//        }
//    }
    Earth* earthObjects[64][64];
    
    //top of field nullptr
    for (int c = 34; c < 64; c++){
        for (int r = 0; r< 59; r++){
            //unique_ptr<Earth> ePtr = make_unique<Earth>Earth(r, c)
            earthObjects[c][r] = nullptr;
        }
    }
    
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

    //fills middle with nullptr
    for (int c = 29; c < 34; c++){
        for (int r = 4; r < 59; r++){
            //unique_ptr<Earth> ePtr = make_unique<Earth>Earth(r, c)
            earthObjects[c][r] = nullptr;
        }
    }

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move(){
    //updateDisplayText();
    Tunnelman* tMan = new Tunnelman();
    tMan->doSomething();
    return GWSTATUS_CONTINUE_GAME;
}

StudentWorld* StudentWorld::getWorld(){
    StudentWorld* S = sWorld;
    return S;
}

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
