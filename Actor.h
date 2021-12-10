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

class Protestor : public Humanoid {
public:
    //default constructor
    Protestor(StudentWorld*, Tunnelman*);
    
    virtual void doSomething();
    
    //virtual bool notPastBoundary(int); //TODO: .
    
    virtual ~Protestor();
    
private:
    int hitPoints;
    int numSquares;
    int current_level_number;
    int ticksToWait;
    int ticks = 0;
    bool leaveTheOil;
    int remainder = 0;
    bool shout = true; //can shout in the beginning
    bool perpTurn = true; //checks to see if there has been a perpendicular turn that has been made in the last 200 non resting ticks
    Tunnelman* tMan;
};

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


#endif // ACTOR_H_
