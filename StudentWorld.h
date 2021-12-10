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
class Protestor;
class Boulder;

class StudentWorld : public GameWorld
{
public:
    // Default constructor
	StudentWorld(std::string assetDir);
    
    virtual int init();
	
    GameWorld* getWorld();
    
    //FOR BOULDER
    bool isEarth(int, int);
    
    bool isBoulder(int, int);
    
    
    //Tunnelman* getTMan();
    
    //void addProtester(int); //might make virtual if you have to add boulders (or any object that requires adding every tick) down the line
    
    //For protestor
    //virtual bool isEarthBoulder(int,int);

    virtual int move();
    
    virtual void cleanUp();
    
    virtual void digEarth(int, int);
    
    virtual void removeEarth(int, int);
    
    virtual void updateDisplayText();
    
    virtual int random(int, int);
    // Destructor
    ~StudentWorld();
    
private:
    Tunnelman* tMan;
    Earth* earthObjects[64][64];
    GameWorld* gWorld;
    //FOR BOULDER
    Boulder* boulder;
    int level;
    //-------------------------
    Protestor* regPro;
    std::vector<Actor*> vActors; //keeps track of all actor pointers
    int ticks = 0;
   
};

#endif // STUDENTWORLD_H_
