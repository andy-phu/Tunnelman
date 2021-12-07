#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>

class Tunnelman;    // Here we only use class forwarding as we don't actually code 
                    //  the implementation of any classes and the likes and simply
                    //  need to be able to declare variables, functions, etc. for 
                    //  use in StudentWorld.cpp. -- Note Line 4 in StudentWorld.cpp
                    //  for further explanation --
class Earth;
class Actor;
class StudentWorld : public GameWorld
{
public:
    // Default constructor
	StudentWorld(std::string assetDir);
    
    virtual int init();
	
    GameWorld* getWorld();
    
    virtual int move();
    
    virtual void cleanUp();
    
    virtual void digEarth(int, int);

    virtual void updateDisplayText();

    // Destructor
    ~StudentWorld();
    
private:
    Tunnelman* tMan;
    Earth* earthObjects[64][64];
    GameWorld* gWorld;

    std::vector<Actor*> vActors;    // Keeps track of all actor pointers
};

#endif // STUDENTWORLD_H_
