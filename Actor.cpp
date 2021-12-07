#include "Actor.h"
#include "StudentWorld.h"
#include <string>
#include <cstdlib>
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
    dead(false); //all actors start alive

}
 
bool Actor::isActor(int x, int y){ //TODO: gonna have to check if it's only an earth or boulder step 7 note says protestors can be same block as other protestors and tunnelman
    if(getX() == x && getY() == y){ //should be able to detect if there is an actor here
        return true;
    }
    return false;
}

bool Actor::dead(bool status){ //lets the derived classes access if they're dead or alive
    return status;
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
Protestor Class
****************************************/
//Default Constructor
Protestor::Protestor(StudentWorld* tempWorld) : Humanoid(TID_PROTESTER, 60, 60, left, 1.0, 0, tempWorld){
     hitPoints = 5;
     leaveTheOil = false; //not in the oil field leave state
     current_level_number = getWorld() -> getLevel(); //gets current level using getLevel game world function
     ticksToWait = max (0, 3 - current_level_number/4); //formula found in pdf
     numSquares = 8 + (rand() % 60); //random number btn 8 and 60
}

void Protestor::doSomething(){
    Tunnelman* tMan = nullptr; //prob doesn't work, might have to do a getter for tunnelman class passing in tunnel man pointer
    
 
    if(ticks / ticksToWait){ //checks if it should be in the resting state, if not do all the checks to do something
        if(leaveTheOil == true){ //try to leave arena
            if(getX() == 60 && getY() == 60){ //at the exit point and can therefore leave
                dead(true); //set status to dead
            }
            else{
                //TODO: dijkstra's to find the exit
            }
        }
        
        //     Else if the Tunnelman is visible via direct line of sight,
        //    then Switch direction to face the Tunnelman Move one square in this direction
        else{ //not trying to leave and all interactions with tunnelman
            //if i can see tunnelman
            if((abs(getX() - tMan->getX()) < 4) || (abs(getY() - tMan->getY()) < 4)) { //checks for a distance of 4 and facing direction of tMan
                Direction direction = getDirection();
                if((direction == left && tMan->getX() < getX()) || (direction == right && tMan->getX() > getX())
                || (direction == down && tMan->getY() < getY()) || (direction == up && tMan->getY() > getY())) //checks if facing same direction
                {
                    if(ticks % 15 == 0 && shout == true){ //if 15 non resting ticks has passed and hasn't shouted yet TODO: keep track of the shouting
                        getWorld()->playSound(SOUND_PROTESTER_YELL); //yell at tunnelman
                        //TODO: deduct two hit points from tunnelman
                        shout = false; //can't shout again for another 15 non resting ticks
                        return;
                    }
                }
                else if(getX() == tMan->getX() && (abs(getY() - tMan->getY()) > 4)){ //same column but distance > 4
                    int distance = getY() - tMan->getY();
                    if(distance < 0){ //when distance is negative tunnelman is above
                        for(int i = getY(); i < tMan->getY(); i++){ //checks the path from protestor to tunnelman
                            if(isActor(getX(), i)){ //checks if there is an actor in the way
                                setDirection(up);
                                moveTo(getX(), getY() + 1); //takes a step towards him upwards
                            }
                        }
                    }
                    else{ //tunnelman is below
                        for(int i = tMan->getY(); i < getY(); i++){ //checks the path from tunnelman to protester
                            if(isActor(getX(), i)){ //checks if there is an actor in the way
                                setDirection(down);
                                moveTo(getX(), getY() - 1); //takes a step towards him downwards
                            }
                        }
                    }
                    return;
                }
                else if(getY() == tMan->getY() && (abs(getX() - tMan->getX()) > 4)){ //same row but distance > 4
                    int distance = getX() - tMan->getX();
                    if(distance < 0){ //when distance is negative tunnelman is right
                        for(int i = getX(); i < tMan->getX(); i++){ //checks the path from protestor to tunnelman
                            if(isActor(i, getY())){ //checks if there is an actor in the way
                                setDirection(right);
                                moveTo(getX() + 1, getY()); //takes a step towards him to the right
                                numSquares = 0; // forces protestor to pick a new direction/distance to move during its next non resting tik
                            }
                        }
                    }
                    else{ //tunnelman is left
                        for(int i = tMan->getX(); i < getX(); i++){ //checks the path from tunnelman to protester
                            if(isActor(i, getY())){ //checks if there is an actor in the way
                                setDirection(left);
                                moveTo(getX() - 1 , getY()); //takes a step towards him to the left
                                numSquares = 0; // forces protestor to pick a new direction/distance to move during its next non resting tik

                            }
                        }
                    }
                    return;
                }
            }
            //code next step 6 here: Otherwise, the Regular Protester can’t directly see the Tunnelman an.
            else{
                numSquares = numSquares - 1;
                //step 6
                if(numSquares <= 0){ //when the reg pro has finished walking numSquares
                    int randomNum = 0;
                    int counter = 0; //to make sure the random number doesn't get chosen more than once
                    while(counter != 4){
                        randomNum = rand()%4; //chooses number from 0-4
                        if(randomNum == 1 && isActor(getX(), getY() + 1)){
                            setDirection(up);
                            counter++;
                        }
                        else if(randomNum == 2 && isActor(getX(), getY() - 1)){
                            setDirection(down);
                            counter++;
                        }
                        else if(randomNum == 3 && isActor(getX() + 1, getY())){
                            setDirection(right);
                            counter++;
                        }
                        else if(randomNum == 4 && isActor(getX() - 1, getY())){
                            setDirection(left);
                            counter++;
                        }
                        else{
                            cout << "no random number was chosen...this is for debugging" << endl;
                        }
                        perpTurn = false; //when a perpendicular turn has been made, make sure it is not made again until 200 ticks has gone by
                    }
                    
                }
                //step 7
                else{
                    //checks for interection
                    if(perpTurn == true){
                        if(getDirection() == up){ //facing up/down, perp directions: left and right
                            if(!(isActor(getX() - 1, getY()))){ //if there is not an actor to the left
                                setDirection(left);
                            }
                            else{
                                setDirection(right);
                            }
                            perpTurn = false;
                        }
                        else if(getDirection() == right){ //facing right/left, perp directions: down and up
                            if(!(isActor(getX(), getY() - 1))){ //if there is not an actor to the left
                                setDirection(down);
                            }
                            else{
                                setDirection(up);
                            }
                            perpTurn = false;
                        }
                        else{
                            cout << "no directions viable, so there is a bug somewhere" << endl;
                        }
                    }
                   
                }
                numSquares = 8 + (rand() % 60); //random number btn 8 and 60
                //step 8 & 9
                if(getDirection() == up){
                    if(!isActor(getX(), getY() + 1)){
                        moveTo(getX(), getY() + 1);
                    }
                }
                else if(getDirection() == down){
                    if(!isActor(getX(), getY() - 1)){
                        moveTo(getX(), getY() - 1);
                    }
                }
                else if(getDirection() == right){
                    if(!isActor(getX() + 1, getY())){
                        moveTo(getX() + 1, getY());
                    }
                }
                else if(getDirection() == left){
                    if(!isActor(getX() - 1, getY())){
                        moveTo(getX() - 1, getY());
                    }
                }
                else{
                    return;
                }
            }
        }
    }
    else{
        ticks++; //keeps incrementing til the appropriate amt of ticks has elapsed
        if(ticks % 15 == 0){ //every 15 reg pro can shout again
            shout = true; 
        }
        if(ticks % 200 == 0){ //every 200 reg pro can do a perpendicular turn again
            perpTurn = true;
        }
    }
    
    
    
}

//Destructor
Protestor::~Protestor(){}




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
Tunnelman::~Tunnelman() {}
