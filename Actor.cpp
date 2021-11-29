#include "Actor.h"
#include <string>
using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

/****************************************
Actor class
****************************************/
// Default constructor
Actor::Actor(int imageID, int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 0) : GraphObject(imageID, startX, startY, startDirection, size, depth) {
    setVisible(true);
}

// Destructor
Actor::~Actor() {

}

/****************************************
Earth class
****************************************/
// Default Constructor
Earth::Earth(int startX, int startY) : Actor(TID_EARTH, startX, startY, right, 0.25, 3) {
    //setVisible(true);
}

// This function does nothing, but we have to set it up due to doSomething in Actor being a pure virtual function
void Earth::doSomething() {

}

// Destructor
Earth::~Earth() {

}

/****************************************
Humanoid class
****************************************/
// Default constructor
Humanoid::Humanoid(int imageID, int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 0) : Actor(imageID, startX, startY, startDirection, size, depth) {

}

// Destructor
Humanoid::~Humanoid() {

}


/****************************************
Tunnelman class
****************************************/
// Default constructor
Tunnelman::Tunnelman(StudentWorld* gWorld) : Humanoid(TID_PLAYER, 30, 60, right, 1.0, 0) {
    fWorld = gWorld;
}

void Tunnelman::doSomething() {
    int ch;

    //student world like to game world to use get key function
    if(fWorld->getKey(ch) == true){
        switch(ch){
            case KEY_PRESS_LEFT:
                if (notPastBoundary(KEY_PRESS_LEFT)) {
                    moveTo(getX() - 1, getY());
                }

                break;
            case KEY_PRESS_RIGHT:
                if (notPastBoundary(KEY_PRESS_RIGHT)) {
                    moveTo(getX() + 1, getY());
                }
                
                break;
            case KEY_PRESS_UP:
                if (notPastBoundary(KEY_PRESS_UP)) {
                    moveTo(getX(), getY() + 1);
                }

                break;
            case KEY_PRESS_DOWN:
                if (notPastBoundary(KEY_PRESS_DOWN)) {
                    moveTo(getX(), getY() - 1);
                }

                break;
        }
    }
}

// Will tell whether or not the current game object is past our game grid boundaries
bool Tunnelman::notPastBoundary(int ch) {
    switch (ch) {
    case KEY_PRESS_LEFT:
        if (getX() - 1 < 0) {
            return false;
        }

        break;
    case KEY_PRESS_RIGHT:
        if (getX() + 1 > 63) {
            return false;
        }

        break;
    case KEY_PRESS_UP:
        if (getY() + 1 > 63) {
            return false;
        }

        break;
    case KEY_PRESS_DOWN:
        if (getY() - 1 < 0) {
            return false;
        }

        break;
    };

    return true;
}

// Destructor
Tunnelman::~Tunnelman() {

}
