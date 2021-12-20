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
            getWorld()->dealDmg(0,0, -100, "Tunnelman");
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

string GoldNugget::objectType() {
    return "GoldNugget";
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

// Will tell whether or not the current game object is past our game grid boundaries OR if it's is
//  running into a oz
bool Humanoid::notPastBoundary(int ch) {
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

bool Humanoid::isAnnoyed(int dmg) {
    if (hitPoints <= 0) {
        getWorld()->decLives();
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
            case 'z':
                // If we have sonar charges to use
                if (inventory[2] > 0) {
                    getWorld()->sonarPing(getX(), getY());
                    getWorld()->playSound(SOUND_SONAR);

                    decrementInventoryCount(2);
                }

                break;
            case 'o':
                cout << "o is pressed" << endl;
                getWorld()->dmgPro();
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

/****************************************
Protestor Class
****************************************/
//Default Constructor
Protester::Protester(StudentWorld* tempWorld, int ID) : Humanoid(ID, 60, 60, left, 1.0, 0, tempWorld) {
    setHitPoints(5);
    leaveTheOil = false; //not in the oil field leave state
    current_level_number = getWorld()->getLevel(); //gets current level using getLevel game world function
    ticksToWait = max(0, 3 - current_level_number / 4); //formula found in pdf
    numSquares = getWorld()->random(8, 60, 'n'); //random number btn 8 and 60
}

string Protester::objectType() {
    return "Protester";
}

void Protester::setLeave() {
    leaveTheOil = true; 
}

void Protester::doSomething() {
    int xPro = getX();
    int yPro = getY();
    int xTMan = getWorld()->getActorObjectX("Tunnelman");
    int yTMan = getWorld()->getActorObjectY("Tunnelman");
    int tDistanceX = xPro - xTMan;
    int tDistanceY = yPro - yTMan;
    Direction dir = getDirection();


   

    if (isDead()) { //if dead return immediately
        return;
    }

    //cout << "SHOUT TICKS " << shoutTicks << endl;
    //increments to keep track of when it can move
    //cout << " TICKS " << ticksToWait << endl;
    //cout << " PRO " << xPro << endl;
    //cout << " shout  " << shoutTicks << endl;

    if (ticksToWait > 0) {
        ticksToWait--;
        return;
    }
    else {
        ticksToWait = max(0, 3 - current_level_number / 4);
    }
    shoutTicks++;
    perpTurnTicks++;
    //cout << "perpTurnTicks " << perpTurnTicks << endl;
    if (xPro == 3) {
        //stuck in top left corner
        moveTo(xPro + 1, yPro);
        setDirection(right);
        return;
    }
    if (xPro == 60 && leaveTheOil == false) {
        //stuck in top left corner
        moveTo(xPro - 1, yPro);
        setDirection(left);
        return;
    }

    if (yPro == 61) {
        moveTo(xPro, yPro - 1);
        setDirection(down);
        return;
    }
    if (yPro == 0) {
        moveTo(xPro, yPro + 1);
        setDirection(up);
        return;
    }


    if (leaveTheOil == true) { //leave the oil phase
        if (getX() == 60 && getY() == 60) { //at the exit point and can therefore leave
            setDead(); //set status to dead
        }
        else {
            getWorld()->exit(this, xPro, yPro, "exit"); //calls function to move the protester one step closer to the exit every round
            return;
        }
    }
    else { //not trying to leave and all interactions with tunnelman
        //if i can see tunnelman
        //cout << "TDISTNAC " << tDistanceX << endl;
        if (radiusCheck(xPro, yPro, xTMan, yTMan, 4)) { //checks for a distance of 4 and facing direction of tMan
            //cout << "test " << endl;
            if (facingDirection(xPro, yPro, xTMan, yTMan, dir)) {
                if (shoutTicks > 15) { //if 15 non resting ticks has passed and hasn't shouted yet TODO: keep track of the shouting
                    getWorld()->playSound(SOUND_PROTESTER_YELL); //yell at tunnelman
                    //TODO: deduct two hit points from tunnelman
                    getWorld()->dealDmg(0,0,-2, "Tunnelman"); 
                    //cout << "YELLING AT TUNNELMAN " << endl;
                    shoutTicks = 0; //can't shout again for another 15 non resting ticks
                    return;
                }
                return;
            }
            else {
                return;
            }
        }
        if (getID() == TID_HARD_CORE_PROTESTER && (tDistanceX > 4 || tDistanceY > 4) ) { //for hardcore protesters that are more than 4 units away from the tunnelman
            int M = 16 + static_cast<int>(current_level_number * 20);
            if (tDistanceX <= M || tDistanceY <= M) { //If the Hardcore Protester is less than or equal to a total of M legal horizontal or vertical moves
                getWorld()->exit(this,xPro, yPro, "find"); //reuse the code but this time mark the tunnelman
            }
        }

        //step 5 for regular protesters 
        if (getX() == getWorld()->getActorObjectX("Tunnelman") && radiusCheck(xPro, yPro, xTMan, yTMan, 4)) { //same column but distance > 4
            cout << "test" << endl;
            bool clearPath = true;
            if (yPro < yTMan) { //when distance is negative tunnelman is above
                for (int i = 0; i < abs(tDistanceY); i++) { //checks the path from protester to tunnelman
                    if (earthBoulderCheck(xPro, yPro + i)) { //checks if there are earth or boulders in the way, hit box would be one, bc protester doesn't get damaged from to
                        clearPath = false;
                        numSquares = 0;
                    }
                }
                if (clearPath == true) {
                    setDirection(up);
                    moveTo(xPro, yPro + 1); //takes a step towards him upwards
                }
            }
            else if (yPro > yTMan) { //tunnelman is below
                for (int i = 0; i < abs(tDistanceY); i++) { //checks the path from tunnelman to protester
                    if (earthBoulderCheck(xPro, yPro - i)) { //checks if there are earth or boulders in the way, hit box would be one, bc protester doesn't get damaged from to
                        clearPath = false;
                        numSquares = 0;
                        
                    }
                }
                if (clearPath == true) {
                    //cout << "test" << endl;
                    setDirection(down);
                    moveTo(xPro, yPro - 1); //takes a step towards him downwards
                }
            }
            cout << "test" << endl;
            numSquares = 0;
            return;
        }
        if (getY() == getWorld()->getActorObjectY("Tunnelman") && radiusCheck(xPro, yPro, xTMan, yTMan, 4)) { //same row but distance > 4
            bool clearPath = true;
            if (xPro < xTMan) { //when distance is negative tunnelman is to the right
                for (int i = 0; i < abs(tDistanceX); i++) { //checks the path from protester to tunnelman
                    if (earthBoulderCheck(xPro + i, yPro) == false) { //checks if there is earth or a boulder in the way
                        clearPath = false; //if there is stuff in the way, clearPath is false
                        numSquares = 0;
                    }
                }
                if (clearPath == true) {
                    setDirection(right);
                    moveTo(xPro + 1, yPro); //takes a step towards him to the right
                }
            }
            else if (xPro > xTMan) { //tunnelman is left
                for (int i = 0; i < abs(tDistanceX); i++) { //checks the path from tunnelman to protester
                    if (earthBoulderCheck(xPro - i, yPro) == false) {
                        clearPath = false;
                        numSquares = 0;
                    }
                }

                if (clearPath == true) {
                    setDirection(left);
                    moveTo(xPro - 1, yPro); //takes a step towards him to the left
                }
            }
            numSquares = 0;
            return;
        }
        numSquares = numSquares - 1;
        //step 6: Otherwise, the Regular Protester can’t directly see the Tunnelman
        if(numSquares <= 0){
           
            //step 7
            bool clear = false;
            //cout << "NUM SQUARES " << numSquares << endl;
            while (clear == false) {
                Direction randomDir = randomDirection(); //chooses random direction
                //checks if the direction is clear
                if (randomDir == up && earthBoulderCheck(xPro, yPro + 1) && notPastBoundary(up)) {
                    setDirection(up);
                    clear = true;
                }
                else if (randomDir == down && earthBoulderCheck(xPro, yPro - 1) && notPastBoundary(down)) {
                    setDirection(down);
                    clear = true;
                }
                else if (randomDir == right && earthBoulderCheck(xPro + 1, yPro) && notPastBoundary(right)) {
                    setDirection(right);
                    clear = true;
                }
                else if (randomDir == left && earthBoulderCheck(xPro - 1, yPro) && notPastBoundary(left)) {
                    setDirection(left);
                    clear = true;
                }
                perpTurnTicks = 0; //when a perpendicular turn has been made, make sure it is not made again until 200 ticks has gone by
            }
            numSquares = getWorld()->random(8, 60, 'n'); //random number btn 8 and 60
        }
        //step 7
        else if (moveInDirection(xPro, yPro, up) && moveInDirection(xPro, yPro, down) && moveInDirection(xPro, yPro, right) && moveInDirection(xPro, yPro, left)) {//checks if it is in an intersection
            //cout << "intersection" << endl;
            //cout << "at an intersection " << perpTurnTicks << endl;
            if (perpTurnTicks > 200) {
                if (dir == right) { //facing right/left, perp directions: down and up
                    if (moveInDirection(xPro, yPro + 1, up)) { //if there is not an actor to the left
                        setDirection(up);
                    }
                    else if (moveInDirection(xPro, yPro - 1, down)) {
                        setDirection(down);
                    }
                    perpTurnTicks = 0;
                }
                if (dir == left) { //facing right/left, perp directions: down and up
                    if (moveInDirection(xPro, yPro - 1, down)) { //if there is not an actor to the left
                        setDirection(down);
                    }
                    else if (moveInDirection(xPro, yPro + 1, up)) {
                        setDirection(up);
                    }
                    perpTurnTicks = 0;
                }
                if (dir == up) { //facing up/down, perp directions: left and right
                    if (moveInDirection(xPro - 1, yPro, left)) { //if there is not an actor to the left
                        setDirection(left);
                    }
                    else if (moveInDirection(xPro + 1, yPro, right)) {
                        setDirection(right);
                    }
                    perpTurnTicks = 0;
                }
                else if (dir == down) { //facing up/down, perp directions: left and right
                    cout << "DOWN" << endl;
                    if (moveInDirection(xPro + 1, yPro, right)) { //if there is not an actor to the left
                        setDirection(right);
                    }
                    else if (moveInDirection(xPro - 1, yPro, left)) {
                        setDirection(left);
                    }
                    perpTurnTicks = 0;
                }
                
            }
            numSquares = getWorld()->random(8, 60, 'n'); //random number btn 8 and 60

        
        }

    }
    //step 8 and checks bounds before moving too
    switch (dir) {
    case up: {
        if (moveInDirection(xPro, yPro, up) && notPastBoundary(up)) {
            moveTo(xPro, yPro + 1);
        }
        break;
    }
    case down: {
        if (moveInDirection(xPro, yPro, down) && notPastBoundary(down)) {
            moveTo(xPro, yPro - 1);
        }
        break;
    }
    case right: {
        if (moveInDirection(xPro, yPro, right) && notPastBoundary(right)) {
            moveTo(xPro + 1, yPro);
        }
        break;
    }
    case left: {
        if (xPro == 3) {
            setDirection(right);
            break;
        }
        else if (moveInDirection(xPro, yPro, left) && notPastBoundary(left)) {
            moveTo(xPro - 1, yPro);
        }
        break;
    }
    case none: {
        cout << "bug in the movement code " << endl;
        setDirection(none);
    }
    }

    //step 9
    if (!moveInDirection(xPro, yPro, dir)) { //if protester cannot move in the direction it is facing
        numSquares = 0; //force it to choose a new direction
    }

}

//uses studentWorld hitbox function to check earth and boulder at the same time for protester
bool Protester::earthBoulderCheck(int x, int y) {
    //actor hitbox call made specifically for protester and searching for earth and boulder
    if (getWorld()->actorsInObjectHitBox(x, y, 0, 0, "Boulder") == 0) {
        return false; //returns false if there is earth or boulder there
    }
    if (getWorld()->actorsInObjectHitBox(x, y, 4, 4, "Earth") == 1)
    {
        return false;
    }
    return true;
}

Actor::Direction Protester::randomDirection() //returns a random direction
{
    int randomNumber = rand() % 4; //4 random directions possible
    switch (randomNumber)
    {
    case 0: {
        return up;
        break;
    }
    case 1: {
        return down;
        break;
    }
    case 2: {
        return left;
        break;
    }
    case 3: {
        return right;
        break;
    }
    }
    return none;
}

bool Protester::moveInDirection(int x, int y, Direction dir) {
    switch (dir) {
    case up: {
        return earthBoulderCheck(x, y + 1); //will return false if you can't move up bc there is either earth or boulder there
        break;
    }
    case down: {
        return earthBoulderCheck(x, y - 1);
    }
    case right: {

        return earthBoulderCheck(x + 1, y);
        break;
    }
    case left: {
        return earthBoulderCheck(x - 1, y);
        break;
    }
    case none: {
        return none;
        break;
    }
    }
    return none;
}

bool Protester::facingDirection(int xP, int yP, int xT, int yT, Direction dir) {
    switch (dir) {
    case up: {
        if (xP == xT && yP<yT) { //if the x same and yP is less than (under) tP than TRUE
            return true;
            break;
        }

    case down: {
        if (xP == xT && yP > yT) {
            return true;
            break;
        }
    }
    case right: {
        if (xP < xT && yP == yT) {
            return true;
            break;
        }
    }
    case left: {
        if (xP > xT && yP == yT) {
            return true;
            break;
        }
    }
    case none: {
        return false;
    }
    }
           return false;

    }
}


//SOURCE: https://stackoverflow.com/questions/27342362/how-to-create-a-circle-collision-detection-c/27342421
bool Protester::radiusCheck(int xP, int yP, int xT, int yT, int radius) {
    return ((xP - xT) * (xP - xT) + (yP - yT) * (yP - yT)) < radius * radius;
}


bool Protester::isAnnoyed(int dmg) {
    if (leaveTheOil == true) {
        return true;
    }
    //means he is annoyed
    //getWorld()->dmgPro(); //temporary for testing
    getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
    stun(); //stuns protester for a certain amt of time
    if(getHitPoints() <= 0){ 
        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        leaveTheOil = true;
        ticksToWait = 0;
        return false; 
        //TODO: point system later
    }
}

void Protester::stun() {
    int N = max(50, 100 - static_cast<int>(getWorld()->getLevel() * 10));
    ticksToWait = N; //makes the ticks to wait the same stun time 
}


//Destructor
Protester::~Protester() {}


/****************************************
Hardcore Protester Class
****************************************/
HardcoreProtester::HardcoreProtester(StudentWorld* tempWorld, int ID) : Protester(tempWorld, ID) {
    setHitPoints(20);
};

string HardcoreProtester::objectType() {
    return "HardcoreProtester";
}