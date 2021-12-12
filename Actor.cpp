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

int Actor::getTick() {
    return tick;
}

void Actor::incrementTick() {
    tick++;
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

/****************************************
Boulder class
****************************************/
// Default Constructor

// TODO: MAY NEED TO CHANGE THIS LATER TO MATCH WITH INVEN ITEM LOGIC
// Num of states of a Boulder: 3
//  - State 1: Stable state
//  - State 2: Waiting state
//  - State 3: Falling state
Boulder::Boulder(int startX, int startY, StudentWorld* tempWorld = nullptr) : Actor(TID_BOULDER, startX, startY, down, 1.0, 1, tempWorld) {
    state = 1;
}

void Boulder::doSomething() {
    if (isDead()) {
        return;
    }

    // Stable state action checks
    if (state == 1) {
        // If there is earth below boulder
        if ((getWorld()->isEarth(getX(), getY() - 1))) {
            return;
        }
        // No earth below boulder
        else {
            // Waiting state
            state = 2;
        }
    }
    // Waiting state and 30 ticks have passed
    else if (state == 2 && ticks % 30 == 0) {
        // Goes to falling state
        state = 3;

        getWorld()->playSound(SOUND_FALLING_ROCK);
    }
    // Falling state
    else if (state == 3) {
        // Keeps moving down each tick
        if (getY() != 0 && !getWorld()->isEarth(getX(), getY() - 1) && getWorld()->actorsInObjectHitBox(getX(), getY(), 4, 4, "Boulder") == -1) {
            moveTo(getX(), getY() - 1);
        }
        else if (getWorld()->actorsInObjectHitBox(getX(), getY(), 3, 3, "Tunnelman") != -1) {
            getWorld()->dealDmg(-100, "Tunnelman");
        }

        else {
            // Deletes once it touches another thing
            setDead();
        }
    }

    // Ticks iterates each time
    ticks++;
}

string Boulder::objectType() {
    return "Boulder";
}

// Destructor
Boulder::~Boulder() {}

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
    Actor(imageID, startX, startY, startDirection, size, depth, tempWorld) {}

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
    setVisible(false);
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
        getWorld()->increaseScore(10);

        // TODO: Tell Tunnelman Object it received a new nugget in it's inventory
        getWorld()->inventoryUpdate(0);
    }

    // TODO: Logic for protestors
    //else if (getObjectState() == 1) {

    //    // If there are protestors then we need to go through each one to see if they are
    //    //  within the hit box of the gold nugget
    //    if (getWorld()->numActorObject("Protestors") > 0) {
    //        vector<
    //    }

    //}

    // Ensure this is a temporary object which will eventually expire
    if (getObjectState() == 1) {
        if (getTick() > 200) {
            setDead();
        }


        incrementTick();
    }
}

GoldNugget::~GoldNugget() {}

/****************************************
Water Class
****************************************/
// Object states do not matter for this class
WaterPool::WaterPool(int startX, int startY, StudentWorld* tempWorld) : invenItems(TID_WATER_POOL, startX, startY, right, 1.0, 2, tempWorld) {
    level = getWorld()->getLevel();
}

void WaterPool::doSomething() {
    if (isDead()) {
        return;
    }

    // When the water pool is within the radius of the tunnelman
    else if ((abs(getX() - getWorld()->getActorObjectX("Tunnelman")) <= 3) && (abs(getY() - getWorld()->getActorObjectY("Tunnelman")) <= 3)) {
        // Gets set to dead when the tunnelman interacts with it
        setDead();

        // Waterpool sound
        getWorld()->playSound(SOUND_GOT_GOODIE);

        getWorld()->inventoryUpdate(1);

        getWorld()->increaseScore(100);
    }

    // When the ticks that left the water pool elapse then water pool dies
    else if (getTick() == max(100, 300 - 10 * level)) {
        setDead();
    }

    // Water pools are temporary objects and are always on a tick timer before despawning
    incrementTick();
}

string WaterPool::objectType() {
    return "WaterPool";
}

WaterPool::~WaterPool() {}

/****************************************
Sonar Class
****************************************/
// Object states in this class do no matter 
SonarKit::SonarKit(int startX, int startY, StudentWorld* tempWorld) : invenItems(TID_SONAR, startX, startY, right, 1.0, 2, tempWorld) {
    level = getWorld()->getLevel();
}

void SonarKit::doSomething() {
    if (isDead()) {
        return;
    }

    else if ((abs(getX() - getWorld()->getActorObjectX("Tunnelman")) <= 3) && (abs(getY() - getWorld()->getActorObjectY("Tunnelman")) <= 3)) {
        // Gets set to dead when the tunnelman interacts with it
        setDead();

        // Pick up sound
        getWorld()->playSound(SOUND_GOT_GOODIE);

        getWorld()->inventoryUpdate(2);

        getWorld()->increaseScore(75);
    }
    else if (getTick() == max(100, 300 - 10 * level)) {
        setDead();
    }

    // Sonar kits are temporary objects and are always on a tick timer before despawning
    incrementTick();
}

string SonarKit::objectType() {
    return "SonarKit";
}

SonarKit::~SonarKit() {}

/****************************************
Humanoid Abstract Base Class
****************************************/
// Default constructor
Humanoid::Humanoid(int imageID, int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 0, StudentWorld* tempWorld = nullptr) : 
    Actor(imageID, startX, startY, startDirection, size, depth, tempWorld) {
    hitPoints = 0;
}

bool Humanoid::isAnnoyed() {
    if (hitPoints <= 0) {
        return true;
    }

    return false;
}

int Humanoid::getHitPoints() {
    return hitPoints;
}

void Humanoid::setHitPoints(int hp) {
    hitPoints += hp;
}

// Destructor
Humanoid::~Humanoid() {}


/****************************************
Tunnelman Class
****************************************/
// Default constructor
Tunnelman::Tunnelman(StudentWorld* tempWorld) : Humanoid(TID_PLAYER, 30, 60, right, 1.0, 0, tempWorld) {
    // Set our Default Hitpoints
    setHitPoints(10);
    
    // Set our default values for our inventory
    inventory[0] = 0;
    inventory[1] = 5;
    inventory[2] = 1;
}

void Tunnelman::doSomething() {
    int ch;

    // Tunnelman Movement:
    // Use getWorld to access class StudentWorld, then further access getKey()
    //  which allows us to read in keyboard input. Then use switch given those inputs
    if (getWorld()->getKey(ch) == true) {
        switch(ch){
            case KEY_PRESS_LEFT:
                
                if (getDirection() != left) {
                    setDirection(left);

                    break;
                }

                if (notPastBoundary(KEY_PRESS_LEFT)) {
                    moveTo(getX() - 1, getY());
                }

                break;
            case KEY_PRESS_RIGHT:
                if (getDirection() != right) {
                    setDirection(right);

                    break;
                }
                
                
                if (notPastBoundary(KEY_PRESS_RIGHT)) {
                    moveTo(getX() + 1, getY());
                }
                
                break;
            case KEY_PRESS_UP:
                if (getDirection() != up) {
                    setDirection(up);

                    break;
                }

                setDirection(up);
                if (notPastBoundary(KEY_PRESS_UP)) {
                    moveTo(getX(), getY() + 1);
                }

                break;
            case KEY_PRESS_DOWN:
                if (getDirection() != down) {
                    setDirection(down);

                    break;
                }

                setDirection(down);
                if (notPastBoundary(KEY_PRESS_DOWN)) {
                    moveTo(getX(), getY() - 1);
                }

                break;
            case KEY_PRESS_TAB:
                // If we have gold nuggets to spare
                if (inventory[0] > 0) {
                    getWorld()->placeGoldNuggets(getX(), getY(), 1, getWorld());

                    decrementInventoryCount(0);
                }

                break;
            case KEY_PRESS_ESCAPE:
                getWorld()->decLives();

                break;
            case 'q':
                exit(0);

                break;
        }
    }

    // Tunnelman Digging:
    getWorld()->digEarth(getX(), getY());
}

// Will tell whether or not the current game object is past our game grid boundaries OR if it's is
//  running into a oz
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

int Tunnelman::getInventoryCount(int item) {
    // - Item 0: Gold
    // - Item 1: Water charges
    // - Item 2: Sonar charges

    return inventory[item];
}

void Tunnelman::incrementInventoryCount(int item) {
    // - Item 0: Gold (+1)
    // - Item 1: Water charges (+5)
    if (item == 1) {
        inventory[item] += 5;

        return;
    }
    // - Item 2: Sonar charges (+1)

    inventory[item]++;
}

void Tunnelman::decrementInventoryCount(int item) {
    // - Item 0: Gold
    // - Item 1: Water charges
    // - Item 2: Sonar charges

    inventory[item]--;
}

string Tunnelman::objectType() {
    return "Tunnelman";
}

// Destructor
Tunnelman::~Tunnelman() {}
