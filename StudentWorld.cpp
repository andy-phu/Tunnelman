#include "StudentWorld.h"
#include <string>
#include <vector>
#include <memory>
using namespace std;

StudentWorld::StudentWorld(string assetDir): GameWorld(assetDir){
    gWorld = this;
    tMan = nullptr;

    //// Set all pointer objects to null so we avoid any errors, and fills in
    ////  both the top and middle of our game grid
    //for (int c = 0; c < 64; c++) {
    //    for (int r = 0; r < 64; r++) {

    //        earthObjects[c][r] = nullptr;
    //    }
    //}
}

//skeleton;
int StudentWorld::init(){
    //tunnelman object getting placed in right spot
    tMan = new Tunnelman(this);

    // Set all pointer objects to null so we avoid any errors, and fills in
//  both the top and middle of our game grid
    for (int c = 0; c < 64; c++) {
        for (int r = 0; r < 64; r++) {

            earthObjects[c][r] = nullptr;
        }
    }

    //push a pointer to an eart object into the vector in each array bucket
    //left of mine shaft in mid rows:0-29 cols:0-59
    for (int c = 0; c < 29; c++){
        for (int r = 0; r< 59; r++){
            earthObjects[c][r] = new Earth(c,r, this);
        }
    }
    
    //right of mine shaft
    for (int c = 34; c < 64; c++){
        for (int r = 0; r< 59; r++){
            earthObjects[c][r] = new Earth(c,r, this);
        }
    }
    
    //earth at the end of mine shaft
    for (int c = 29; c < 34; c++){
        for (int r = 0; r< 4; r++){
            earthObjects[c][r] = new Earth(c,r, this);
        }
    }

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move(){
    updateDisplayText();
    tMan->doSomething();
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
    // Delete tunnelman object
    delete tMan;

    // Delete all remaining earth objects
    for (int c = 0; c < 64; c++) {
        for (int r = 0; r < 64; r++) {
            delete earthObjects[c][r];
        }
    }
    
    // TODO: Finish code here
}

void StudentWorld::digEarth(int x, int y) {
    // Check if there are any earth blocks within the Tunnelman objects hitbox (4x4 area)
    if (earthObjects[x][y] != nullptr || earthObjects[x + 1][y + 1] != nullptr || 
        earthObjects[x + 2][y + 2] != nullptr || earthObjects[x + 3][y + 3] != nullptr) {
        // Dig out any blocks 3x3 from Tunnelman objects actual X Y point
        for (int c = 0; c < 4; c++) {
            for (int r = 0; r < 4; r++) {
                delete earthObjects[x + c][y + r];
                earthObjects[x + c][y + r] = nullptr;
            }
        }

        // Dig out any blocks from Tunnelman objects actual X Y point
        delete earthObjects[x][y];
        earthObjects[x][y] = nullptr;

        playSound(SOUND_DIG);
    }
}

void StudentWorld::updateDisplayText() {
    string displayText;

    displayText = "X: ";
    displayText += to_string(tMan->getX());
    displayText += "\tY: ";
    displayText += to_string(tMan->getY());

    setGameStatText(displayText);
}

GameWorld* StudentWorld::getWorld(){
    return gWorld;
}

StudentWorld::~StudentWorld() {
    // Delete tunnelman object
    delete tMan;

    // Delete all remaining earth objects
    for (int c = 0; c < 64; c++) {
        for (int r = 0; r < 64; r++) {
            delete earthObjects[c][r];
        }
    }
}

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}


// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
