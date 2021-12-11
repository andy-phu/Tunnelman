#include "Actor.h"
#include "StudentWorld.h"
#include <string>
#include <math.h>
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

    // Actors start alive
    dead = false;
}

// Returns StudentWorld object which has been passed into our Actor object through arguments
StudentWorld* Actor::getWorld() {
    return gWorld;
}

string Actor::objectType() {
    return "Actor";
}

void Actor::setDead() {
    dead = true;
    setVisible(false);
}

bool Actor::isDead() {
    return dead;
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

string Earth::objectType() {
    return "Earth";
}

// Destructor
Earth::~Earth() {}

///****************************************
//Boulder class
//****************************************/
//// Default Constructor
//Boulder::Boulder(int startX, int startY, StudentWorld* tempWorld = nullptr) : Actor(TID_BOULDER, startX, startY, down, 1.0, 1, tempWorld) {
//    // Starts at stable state (1), waiting state (2), falling state (3)
//    state = 1;
//}
//
//string Boulder::objectType() {
//    return "Boulder";
//}
//
//void Boulder::doSomething() {
//    if (isDead()) {
//        return;
//    }
//
//    // Stable state action checks
//    if (state == 1) {
//        // If there is earth below boulder
//        if ((getWorld()->isEarth(getX(), getY() - 1)) && (getWorld()->isEarth(getX() + 1, getY() - 1)) && (getWorld()->isEarth(getX() + 2, getY() - 1)) &&
//            (getWorld()->isEarth(getX() + 3, getY() - 1))) {
//            return;
//        }
//        // No earth below boulder
//        else {
//            // Waiting state
//            state = 2;
//        }
//    }
//    // Waiting state and 30 ticks have passed
//    else if (state == 2 && ticks % 30 == 0) {
//        // Goes to falling state
//        state = 3;
//
//        getWorld()->playSound(SOUND_FALLING_ROCK);
//    }
//    // Falling state
//    else if (state == 3) {
//        if (getY() != 0 && !getWorld()->isEarth(getX(), getY() - 1) && (!getWorld()->isBoulder(getX(), getY() - 4))) {
//            // Keeps moving down each tick
//            moveTo(getX(), getY() - 1);
//        }
//        else {
//            // Deletes once it touches another thing
//            setDead();
//        }
//    }
//
//    // Ticks iterates each time
//    ticks++;
//}
//
//// Destructor
//Boulder::~Boulder() {}

/****************************************
Barrel of oil
****************************************/
BarrelOfOil::BarrelOfOil(int startX, int startY, StudentWorld* tempWorld) : Actor(TID_BARREL, startX, startY, right, 1.0, 2, tempWorld) {
    setVisible(false);
}

void BarrelOfOil::doSomething() {
    // Barrel is no longer on the field, so do nothing
    if (isDead()) {
        return;
    }

    // Barrel is still on the field, do something
    if (!isVisible() &&  (abs(getWorld()->getActorObjectX("Tunnelman") - getX()) <= 4 && abs(getWorld()->getActorObjectY("Tunnelman") - getY()) <= 4)) {
        setVisible(true);
        return;
    }


    else if (abs(getWorld()->getActorObjectX("Tunnelman") - getX()) <= 3 && abs(getWorld()->getActorObjectY("Tunnelman") - getY()) <= 3) {
        setDead();
        getWorld()->increaseScore(1000);
        getWorld()->playSound(SOUND_FOUND_OIL);
    }
}

string BarrelOfOil::objectType() {
    return "BarrelOfOil";
}

BarrelOfOil::~BarrelOfOil() {}

/****************************************
Squirt Class
****************************************/
Squirt::Squirt(int startX, int startY, Direction dir, StudentWorld* tempWorld) : Actor(TID_WATER_SPURT, startX, startY, dir, 1.0, 1, tempWorld) {}

void Squirt::doSomething() {

}

string Squirt::objectType() {
    return "Squirt";
}

Squirt::~Squirt() {}

/****************************************
invenItems Abstract Base Class
****************************************/
invenItems::invenItems(int imageID, int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 0, StudentWorld* tempWorld = nullptr) :
    Actor(imageID, startX, startY, startDirection, size, depth, tempWorld) {

}

int invenItems::getObjectState() {
    return objectState;
}

void invenItems::setObjectState(int updatedState) {
    objectState = updatedState;
}

invenItems::~invenItems() {}

/****************************************
GoldNugget Class
****************************************/
// Num of states of a gold nugget: 2
//  - State 0: Pickupable by Tunnelman
//  - State 1: Picupable by protestors
GoldNugget::GoldNugget(int startX, int startY, int state, StudentWorld* tempWorld) : invenItems(TID_GOLD, startX, startY, right, 1.0, 2, tempWorld) {
    setObjectState(state);
}

void GoldNugget::doSomething() {
    // Gold nugget is no longer in the field
    if (isDead()) {
        return;
    }

    // State 0:
    // The nugget is hidden under the earth
    if (!isVisible() && (abs(getWorld()->getActorObjectX("Tunnelman") - getX()) <= 4 && abs(getWorld()->getActorObjectY("Tunnelman") - getY()) <= 4)) {
        setVisible(true);
        return;
    }

    // State 0: 
    // The nugget is going to be picked up by the Tunnelman
    else if (getObjectState() == 0 && (abs(getWorld()->getActorObjectX("Tunnelman") - getX()) <= 3 && abs(getWorld()->getActorObjectY("Tunnelman") - getY()) <= 3)) {
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);

        // TODO: Increase players score by ten points!!

        // TODO: Tell Tunnelman Object it received a new nugget in it's inventory
    }

    /*else if (getObjectState() == 1 &&)*/
}

GoldNugget::~GoldNugget() {}

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

            case KEY_PRESS_ESCAPE:
                getWorld()->decLives();

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

string Tunnelman::objectType() {
    return "Tunnelman";
}

// Destructor
Tunnelman::~Tunnelman() {}
