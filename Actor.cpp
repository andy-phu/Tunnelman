#include "Actor.h"
#include <string>
using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

/****************************************
Actor class
****************************************/
// Default constructor
Actor::Actor(int imageID, int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 0) : GraphObject(imageID, startX, startY, startDirection, size, depth) {
    
}

// Destructor
Actor::~Actor() {

}

StudentWorld* Actor::world(){
    return sWorld;
}

/****************************************
Earth class
****************************************/
// Default Constructor
Earth::Earth(int startX, int startY) : Actor(TID_EARTH, startX, startY, right, 0.25, 3) {
    setVisible(true);
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
Tunnelman::Tunnelman() : Humanoid(TID_PLAYER, 30, 60, right, 1.0, 0) {
    setVisible(true);
}

void Tunnelman::doSomething() {
    int ch;
    StudentWorld* sWorld = world();
    //student world like to game world to use get key function
    if(sWorld->getWorld()->getKey(ch) == true){
        switch(ch){
            case KEY_PRESS_LEFT:
                moveTo(getX()-1, getY());
                break;
            case KEY_PRESS_RIGHT:
                moveTo(getX()+1, getY());
                break;
            case KEY_PRESS_UP:
                moveTo(getX(), getY()+1);
                break;
            case KEY_PRESS_DOWN:
                moveTo(getX(), getY()-1);
                break;
        }
    }
}

// Destructor
Tunnelman::~Tunnelman() {

}
