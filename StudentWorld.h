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
class Protester;
class HardcoreProtester;
class GraphObject;

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

    void digEarth(int, int);

    void sonarPing(int, int);

    void addingProtester();

    virtual void removeEarth(int, int);

    bool isEarth(int, int);

    bool isBoulder(int, int);

    virtual int getActorObjectX(std::string);

    virtual int getActorObjectY(std::string);

    int actorsInObjectHitBox(int, int, int, int, std::string);

    virtual int numActorObject(std::string);

    virtual void inventoryUpdate(int);

    void dealDmg(int, int, int, std::string);

    virtual void updateDisplayText();

    virtual int random(int, int, char);

    void exit(Protester*, int, int, std::string);
    
    void proMove(int, int, Protester*);

    void dmgPro();

    // Destructor
    ~StudentWorld();
    
private:
    Tunnelman* tMan;
    Earth* earthObjects[64][64];
    GameWorld* gWorld;
    Boulder* boulder;
    Protester* regPro;
    HardcoreProtester* hardPro;

    int ticks;
    int currLife;
    int level;
    int lives;
    int health;
    int squirts;
    int gold;
    int barrelsLeft;
    int sonar;
    int score;

    //for maze solver
    int map[64][64];

    struct grid
    {
        int x;
        int y;
        grid(int col, int row){
            x = col;
            y = row;
        }
    };

    // Keeps track of all actor pointers
    std::vector<Actor*> vActors;
};

#endif // STUDENTWORLD_H_
