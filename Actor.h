#ifndef ACTOR_H
#define ACTOR_H

#include <string>

#include "GraphObject.h"

class StudentWorld; 
class Tunnelman;

/****************************************
Actor Abstract Base Class
****************************************/
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
// Actor - Abstract base class for all game Objects
class Actor: public GraphObject {
public:
    // Default constructor
    Actor(int, int, int, Direction, float, unsigned int, StudentWorld*);

    // This function serves as the default most action that any game object will perform
    virtual void doSomething() = 0;

    virtual StudentWorld* getWorld();

    virtual std::string objectType();

    virtual void setDead();

    virtual bool isDead();

    int getTick();

    virtual int getHitPoints() { return 0;  };

    virtual bool isAnnoyed(int) { return true; };

    virtual void setHitPoints(int) {}; //so that we can use the vector of actors to call this function that is specialized for humanoids

    void incrementTick();

    // Destructor
    virtual ~Actor();
private:
    StudentWorld* gWorld;
    bool dead;
    int tick = 0;
};

/****************************************
Earth Class
****************************************/
class Earth : public Actor {
public:
    // Default constructor
    Earth(int, int, StudentWorld*);

    // This function does nothing
    virtual void doSomething();

    virtual std::string objectType();

    // Destructor
    virtual ~Earth();
};

/****************************************
Boulder Class
****************************************/
class Boulder : public Actor {
public:
    Boulder(int, int, StudentWorld*);

    virtual std::string objectType();

    virtual void doSomething();

    virtual ~Boulder();

private:
    int state;
    int ticks = 0;
    Tunnelman* playerObj;
};

/****************************************
Barrel of oil Class
****************************************/
class BarrelOfOil : public Actor {
public:
    BarrelOfOil(int, int, StudentWorld*);

    virtual void doSomething();

    virtual std::string objectType();

    virtual ~BarrelOfOil();
};

/****************************************
Squirt Class
****************************************/
class Squirt : public Actor {
public:
    Squirt(int, int, Direction, StudentWorld*);

    virtual void doSomething();

    virtual std::string objectType();

    virtual ~Squirt();
private:
    int travelDistance = 4;
};

/****************************************
invenItems Abstract Base Class
****************************************/
class invenItems : public Actor {
public:
    invenItems(int, int, int, Direction, float, unsigned int, StudentWorld*);

    // virtual std::string objectType();

    int getObjectState();

    void setObjectState(int);

    virtual ~invenItems();
private:
    // Objects of this type may have multiple types, which can be specified within the derived class itself
    int objectState = 0;
};

/****************************************
GoldNugget Class
****************************************/
class GoldNugget : public invenItems {
public:
    // Default Constructor
    GoldNugget(int, int, int, StudentWorld*);

    virtual void doSomething();

    virtual std::string objectType();

    // Destructor
    ~GoldNugget();
};

/****************************************
Water Class
****************************************/
class WaterPool : public invenItems {
public:
    // Default Constructor
    WaterPool(int, int, StudentWorld*);

    virtual void doSomething();

    virtual std::string objectType();

    virtual ~WaterPool();

private:
    int level;
};


/****************************************
Sonar Class
****************************************/
class SonarKit : public invenItems {
public:
    SonarKit(int, int, StudentWorld*);

    virtual void doSomething();

    virtual std::string objectType();

    virtual ~SonarKit();

private:
    int level;
};

/****************************************
Humanoid Abstract Base Class
****************************************/
// Humanoid - base class used for all human type objects within the game
class Humanoid : public Actor {
public:
    // Default construtor
    Humanoid(int, int, int, Direction, float, unsigned int, StudentWorld*);

    // virtual void doSomething() = 0;    // Leaving this line to signify that we are still inheritting this function as a
                                          //    pure virtual function from Actor
                                       
    virtual bool notPastBoundary(int);

    
    virtual bool isAnnoyed(int);            // TODO: Will not implement this yet, leaving in to show this is not just going to
                                     //    be a copy and paste of Actor.

    virtual int getHitPoints();

    // This function will be used to both set default Hit Points value upon spawn
    //  and also deal dmg to the objects hit points. 
    virtual void setHitPoints(int);

    // Destructor
    virtual ~Humanoid();

private:
    int hitPoints;
};

/****************************************
Tunnelman Class
****************************************/
// Tunnelman - Gameobject that will be used for in game character Tunnelman, notice this class is final
//    meaning that no other classes may inherit from this class.
class Tunnelman : public Humanoid {
public:
    //Default constructor
    Tunnelman(StudentWorld*);

    virtual void doSomething();

    virtual bool notPastBoundary(int);

    virtual int getInventoryCount(int);

    virtual void incrementInventoryCount(int);

    virtual void decrementInventoryCount(int);

    virtual std::string objectType();

    virtual ~Tunnelman();

private:
    // The Tunnelman holds 3 items
    //  - Item 0: Gold
    //  - Item 1: Water
    //  - Item 2: Sonar Charges
    int inventory[3];
};

/****************************************
Protestor Class
****************************************/
class Protester : public Humanoid {
public:
    //default constructor
    Protester(StudentWorld*);

    virtual std::string objectType();

    virtual void setLeave();

    virtual bool isAnnoyed(int); 

    void stun();

    virtual void doSomething();

    bool earthBoulderCheck(int, int);

    Direction randomDirection();

    bool moveInDirection(int, int, Direction);

    bool facingDirection(int, int, int, int, Direction);

    bool radiusCheck(int, int, int, int, int);

    //virtual bool notPastBoundary(int);

    virtual ~Protester();

private:
    int hitPoints;
    int numSquares;
    int current_level_number;
    int ticksToWait;
    int shoutTicks = 15;
    int perpTurnTicks = 200;
    bool leaveTheOil;
    int remainder = 0;
    bool shout = true; //can shout in the beginning
    bool perpTurn = true; //checks to see if there has been a perpendicular turn that has been made in the last 200 non resting ticks

};


#endif // ACTOR_H_
