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
class Boulder;

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

    virtual void removeEarth(int, int);

    //bool isEarth(int, int);

    //bool isBoulder(int, int);

    virtual int getActorObjectX(std::string);

    virtual int getActorObjectY(std::string);

    virtual int numActorObject(std::string);

    virtual void updateDisplayText();

    /*virtual int random(int, int);*/

    // Destructor
    ~StudentWorld();
    
private:
    Tunnelman* tMan;
    Earth* earthObjects[64][64];
    GameWorld* gWorld;
    Boulder* boulder;


    int level;
    int lives;
    int health;
    int squirts;
    int gold;
    int barrelsLeft;
    int sonar;
    int score;

    // Keeps track of all actor pointers
    std::vector<Actor*> vActors;

    int ticks = 0;
};

#endif // STUDENTWORLD_H_
