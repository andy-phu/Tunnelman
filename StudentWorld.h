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
class Protestor;

class StudentWorld : public GameWorld
{
public:
    // Default constructor
	StudentWorld(std::string assetDir);
    
    virtual int init();
	
    // Returns a pointer to the current StudentWorld that this function is being
    //  called from
    GameWorld* getWorld();

    virtual int move();
    
    virtual void cleanUp();
    
    void removeDeadGameObjects();
    
    bool playerDiedDuringThisTick();

    void placeGoldNuggets(int, int, int, StudentWorld*);

    virtual void digEarth(int, int);

    virtual void removeEarth(int, int);

    bool isEarth(int, int);

    bool isBoulder(int, int);

    virtual int getActorObjectX(std::string);

    virtual int getActorObjectY(std::string);

    int actorsInObjectHitBox(int, int, int, int, std::string);

    virtual int numActorObject(std::string);

    virtual void inventoryUpdate(int);

    void dealDmg(int, std::string);

    virtual void updateDisplayText();

    virtual int random(int, int, char);

    // Destructor
    ~StudentWorld();
    
private:
    Tunnelman* tMan;
    Earth* earthObjects[64][64];
    GameWorld* gWorld;
    Boulder* boulder;
    Protestor* regPro;
    
    int currLife;
    int T;
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
