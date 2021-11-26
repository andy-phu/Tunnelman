#include "Actor.h"
#include "StudentWorld.h"

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

/****************************************
Earth class
****************************************/
// Default Constructor
Earth::Earth(int imageID, int startX, int startY, Direction startDirection, float size = 0.25, unsigned int depth = 3) : Actor(imageID, startX, startY, startDirection, size, depth) {
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
Tunnelman::Tunnelman(int imageID, int startX, int startY, Direction startDirection, float size = 1.0, unsigned int depth = 0) : Humanoid(imageID, startX, startY, startDirection, size, depth) {

}

void Tunnelman::doSomething() {

}

// Destructor
Tunnelman::~Tunnelman() {

}