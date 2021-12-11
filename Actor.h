#ifndef ACTOR_H
#define ACTOR_H

#include "GraphObject.h"

class StudentWorld; 
class Tunnelman;

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
    
    virtual bool isActor(int, int); //checks to see if there is an actor at certain location
    
    
    // Destructor
    virtual ~Actor();
private:
    int imageID;
    StudentWorld* gWorld;
    Actor* actor;
    bool dead; 
};

class Earth : public Actor {
public:
    // Default constructor
    Earth(int, int, StudentWorld*);

    // This function does nothing
    virtual void doSomething();
    
    
    // Destructor
    virtual ~Earth();
};

// Humanoid - base class used for all human type objects within the game
class Humanoid : public Actor {
public:
    // Default construtor
    Humanoid(int, int, int, Direction, float, unsigned int, StudentWorld*);

    // virtual void doSomething() = 0;    // Leaving this line to signify that we are still inheritting this function as a
                                          //    pure virtual function from Actor
                                       
    /*virtual bool notPastBoundary();*/
    
    // bool isAnnoyed() = 0;            // TODO: Will not implement this yet, leaving in to show this is not just going to
                    
    //    be a copy and paste of Actor.
    // Destructor
    virtual ~Humanoid();
};

//class Protestor : public Humanoid {
//public:
//    //default constructor
//    Protestor(StudentWorld*, Tunnelman*);
//    
//    virtual void doSomething();
//
//    //virtual bool notPastBoundary(int); //TODO: .
//
//    virtual ~Protestor();
//
//private:
//    int hitPoints;
//    int numSquares;
//    int current_level_number;
//    int ticksToWait;
//    int ticks = 0;
//    bool leaveTheOil;
//    int remainder = 0;
//    bool shout = true; //can shout in the beginning
//    bool perpTurn = true; //checks to see if there has been a perpendicular turn that has been made in the last 200 non resting ticks
//    Tunnelman* tMan;
//};

// Tunnelman - Gameobject that will be used for in game character Tunnelman, notice this class is final
//    meaning that no other classes may inherit from this class.
class Tunnelman : public Humanoid {
public:
    //Default constructor
    Tunnelman(StudentWorld*);

    virtual std::string objectType();
    
    virtual void doSomething();

    virtual bool notPastBoundary(int);

    virtual ~Tunnelman();

};

class Boulder : public Actor {
public:
    Boulder(int, int, StudentWorld*);
    
    virtual std::string objectType();

    virtual void doSomething();

    //virtual ~Boulder();

private:
    int state;
    int ticks = 0; 
    Tunnelman* playerObj;
    
};




/****************************************
Barrel of oil
****************************************/
class BarrelOfOil : public Actor {
public:
    BarrelOfOil(int, int, Tunnelman*, StudentWorld*);
    
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
    Tunnelman* playerObj;
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

    // Destructor
    ~GoldNugget();
};

/****************************************
Water Pool
****************************************/
class WaterPool : public invenItems {
public:
    
    WaterPool(int, int, int, StudentWorld*);

    virtual void doSomething();

    virtual std::string objectType();

    virtual ~WaterPool();

private:
    int T; //ticks that a water pool can exist
    int ticks = 0;
};

/****************************************
Sonar Kit
****************************************/
class SonarKit : public invenItems {
public:
    
    SonarKit(int, int, int, StudentWorld*);

    virtual void doSomething();

    virtual std::string objectType();

    virtual ~SonarKit();

private:
    int T; //ticks that a water pool can exist
    int ticks = 0;
};

#endif // ACTOR_H_
