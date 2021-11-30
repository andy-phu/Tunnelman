#include "Actor.h"
#include "StudentWorld.h"
#include <string>
using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

/****************************************
Actor Abstract Base Class
****************************************/
// Default constructor
Actor::Actor(int imageID, int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 0, StudentWorld* tempWorld = nullptr) 
    : GraphObject(imageID, startX, startY, startDirection, size, depth) {

    setVisible(true);
    gWorld = tempWorld;
}

// Returns StudentWorld object which has been passed into our Actor object through arguments
StudentWorld* Actor::getWorld() {
    return gWorld;
}

// Destructor
Actor::~Actor() {}

/****************************************
Earth Class
****************************************/
// Default Constructor
Earth::Earth(int startX, int startY, StudentWorld* tempWorld = nullptr) : Actor(TID_EARTH, startX, startY, right, 0.25, 3, tempWorld) {}

// This function does nothing, but we have to set it up due to doSomething in Actor being a pure virtual function
void Earth::doSomething() {}

// Destructor
Earth::~Earth() {}

/****************************************
Humanoid Abstract Base Class
****************************************/
// Default constructor
Humanoid::Humanoid(int imageID, int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 0, StudentWorld* tempWorld = nullptr) : 
    Actor(imageID, startX, startY, startDirection, size, depth, tempWorld) {}

// Destructor
Humanoid::~Humanoid() {}


/****************************************
Tunnelman Class
****************************************/
// Default constructor
Tunnelman::Tunnelman(StudentWorld* tempWorld) : Humanoid(TID_PLAYER, 30, 60, right, 1.0, 0, tempWorld) {}

void Tunnelman::doSomething() {
    int ch;

    // Tunnelman Movement:
    // Use getWorld to access class StudentWorld, then further access getKey()
    //  which allows us to read in keyboard input. Then use switch given those inputs
    if (getWorld()->getKey(ch) == true) {
        switch(ch){
            case KEY_PRESS_LEFT:
                setDirection(left);
                if (notPastBoundary(KEY_PRESS_LEFT)) {
                    moveTo(getX() - 1, getY());
                }

                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                if (notPastBoundary(KEY_PRESS_RIGHT)) {
                    moveTo(getX() + 1, getY());
                }
                
                break;
            case KEY_PRESS_UP:
                setDirection(up);
                if (notPastBoundary(KEY_PRESS_UP)) {
                    moveTo(getX(), getY() + 1);
                }

                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                if (notPastBoundary(KEY_PRESS_DOWN)) {
                    moveTo(getX(), getY() - 1);
                }

                break;
        }
    }

    // Tunnelman Digging:
    getWorld()->digEarth(getX(), getY());
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
        if (getX() + 1 > 60) {
            return false;
        }

        break;
    case KEY_PRESS_UP:
        if (getY() + 1 > 60) {
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
