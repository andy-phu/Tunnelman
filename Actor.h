#ifndef ACTOR_H
#define ACTOR_H
#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
// Actor - Abstract base class for all game Objects
class Actor: public GraphObject {
public:
    // Default constructor
    Actor(int, int, int, Direction, float, unsigned int);

    // This function serves as the default most action that any game object will perform
    virtual void doSomething() = 0;

    // Destructor
    virtual ~Actor();
};

class Earth : public Actor {
public:
    // Default constructor
    Earth(int, int);

    // This function does nothing
    virtual void doSomething();

    // Destructor
    virtual ~Earth();
};

// Humanoid - base class used for all human type objects within the game
class Humanoid : public Actor {
public:
    // Default construtor
    Humanoid(int, int, int, Direction, float, unsigned int);

    // virtual void doSomething() = 0;    // Leaving this line to signify that we are still inheritting this function as a
                                        //    pure virtual function from Actor

    // bool isAnnoyed() = 0;            // TODO: Will not implement this yet, leaving in to show this is not just going to
                                        //    be a copy and paste of Actor.

    // Destructor
    virtual ~Humanoid();
};

// Tunnelman - Gameobject that will be used for in game character Tunnelman, notice this class is final
//    meaning that no other classes may inherit from this class.
class Tunnelman : public Humanoid {
public:
    //Default constructor
    Tunnelman();

    virtual void doSomething();

    virtual ~Tunnelman();
};

#endif // ACTOR_H_
