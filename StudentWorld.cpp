#include "StudentWorld.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include "Actor.h"  // We use this in the .cpp file instead of .h file due to wanting the actual implementation of Actor.h
                    //  classes, member functions and the like to be imported into this file for actual use. Due to coding
                    //  actual implementation of classes and the likes of StudentWorld.h in this file. -- Note line 9 in 
                    //  StudentWorld.h for further explanation. --
#include <memory>
using namespace std;

StudentWorld::StudentWorld(string assetDir): GameWorld(assetDir){
    // Used for setting up any and all positions of game objects utilizing min
    // int min;
    currLife = getLives();
    level = getLevel();
    // this gives the address of this StudentWorld object
    gWorld = this;
    tMan = nullptr;

    // Set all pointer objects to null so we avoid any errors, and fills in
    //  both the top and middle of our game grid
    for (int c = 0; c < 64; c++) {
        for (int r = 0; r < 64; r++) {
            earthObjects[c][r] = nullptr;
        }
    }
}

int StudentWorld::init(){
    //tunnelman object getting placed in right spot
    tMan = new Tunnelman(this);

    /********************
    Place all earth blocks and tunnel shaft
    ********************/
    //push a pointer to an eart object into the vector in each array bucket
    //left of mine shaft in mid rows:0-29 cols:0-59
    for (int c = 0; c < 30; c++){
        for (int r = 0; r< 60; r++){
            earthObjects[c][r] = new Earth(c, r, this);
        }
    }
    
    //right of mine shaft
    for (int c = 34; c < 64; c++){
        for (int r = 0; r< 60; r++){
            earthObjects[c][r] = new Earth(c, r, this);
        }
    }
    
    //earth at the end of mine shaft
    for (int c = 30; c < 34; c++){
        for (int r = 0; r< 4; r++){
            earthObjects[c][r] = new Earth(c,r, this);
        }
    }

    /********************
    Place all Barrels of Oil
    ********************/
    // TODO: make barrels of oil num actually implement as required 
    // Set our rand seed to create pseudo random numbers
    // min = min(level / 2 + currLevel, 21);

    for (int i = 0; i < min(2 + level, 21); i++) {
        int randX = random(0, 60, 'X');
        int randY = random(20, 56, 'Y');

        vActors.push_back(new BarrelOfOil(randX, randY, this));
    }

    

    /********************
    Place the boulders
    ********************/
    int B = min(level / 2 + 2, 9);
    int counter = 0;

    // Adds the right amt of boulders for each level
    while (counter != B) {
        int randX;

        randX = random(0, 60, 'X');

        // 20 to 56
        int randY = random(0, 56, 'Y');

        // Places boulder
        boulder = new Boulder(randX, randY, this);



        // Removes earth in a 4x4 square for boulder
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                removeEarth(randX + i, randY + j);
                //removeEarth(randX + i, randY + j);
                //removeEarth(randX + i, randY + j);
                //removeEarth(randX + i, randY + j);
            }
        }

        vActors.push_back(boulder);

        counter++;
    }


    //// USE FOR TESTING BOULDERS ON BOULDERS
    //vActors.push_back(new Boulder(35, 56, this));

    //for (int i = 0; i < 4; i++) {
    //    for (int j = 0; j < 4; j++) {
    //        removeEarth(35 + i, 56 + j);
    //        //removeEarth(randX + i, randY + j);
    //        //removeEarth(randX + i, randY + j);
    //        //removeEarth(randX + i, randY + j);
    //    }
    //}

    //vActors.push_back(new Boulder(35, 30, this));

    //for (int i = 0; i < 4; i++) {
    //    for (int j = 0; j < 4; j++) {
    //        removeEarth(35 + i, 30 + j);
    //        //removeEarth(randX + i, randY + j);
    //        //removeEarth(randX + i, randY + j);
    //        //removeEarth(randX + i, randY + j);
    //    }
    //}

    /********************
    Place gold nuggets
    ********************/
    for (int i = 0; i < max(5 - level / 2, 2); i++) {
        int randX = random(0, 60, 'X');
        int randY = random(0, 56, 'Y');

        placeGoldNuggets(randX, randY, 0, this);
    }
    
    /********************
    Place first protester in the very first tick of each level
    ********************/
    regPro = new Protester(this);
    vActors.push_back(regPro);
    int T = max(25, 200 - level);


    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move(){
    // UPDATE THE GAME STATUS LINE //

    // update the display text first as this also updates many of the private variables that the logic below
    //  may rely on. 
    updateDisplayText();
    
    // GIVE EACH ACTOR A CHANCE TO DO SOMETHING // 

    // TODO: Check that the objects are alive!!
    tMan->doSomething();
    
    for (int i = 0; i < vActors.size(); i++) {
        vActors[i]->doSomething();
    }

    // SPAWN IN NEW OBJECTS
    // 1 in G chance
    int G = level * 25 + 300;
    if (random(0, G, 'r') == G) {
        // 4 / 5 chance of water pools
        if (random(0, 5, 'r') < 5) {
            int randX = random(0, 64, 'W');
            int randY = random(0, 64, 'W');

            // There are no earth blocks in the hitbox of where the water pool wishes to
            //  spawn
            if (actorsInObjectHitBox(randX, randY, 4, 4, "Earth") == -1) {
                vActors.push_back(new WaterPool(randX, randY, this));
            }
        }
        // The num is equal to 5: 1 / 5 chance of Sonar kits
        else {
            vActors.push_back(new SonarKit(0, 60, this));
        }
    }

    // REMOVE NEWLY-DEAD ACCTORS AFTER EACH TICK
    removeDeadGameObjects();

    // RETURN THE PROPER RESULT
    if (playerDiedDuringThisTick()) {
        return GWSTATUS_PLAYER_DIED;
    }

    // IF THE PLAYER HAS COLLECTED ALL OF THE BARRELS ON THE LEVEL, THEN 
    //  RETURN THE RESULT THAT THE PLAYER FINISHED THE LEVEL
    if (numActorObject("BarrelOfOil") == 0) {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }

    // THE PLAYER HASN'T COMPLETED THE CURRENT LEVEL AND HASN'T DIED
    //  LET THEM CONTINUE PLAYING THE CURRENT LEVEL
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
    Actor* temp = nullptr;
    
    // Delete tunnelman object
    delete tMan;

    // Delete all remaining earth objects
    for (int c = 0; c < 64; c++) {
        for (int r = 0; r < 64; r++) {
            if (earthObjects[c][r] != nullptr) {
                delete earthObjects[c][r];
            }
        }
    }

    // Delete all remaining vActor objects
    while (!vActors.empty()) {
        temp = vActors[0];
        vActors.erase(vActors.begin());
        delete temp;
    }
}

// This function is used in tandem with the move() function and should thus only be called by said 
//  function 
void StudentWorld::removeDeadGameObjects() {
    Actor* temp;    // This will hold our object which we are to delete from memory
    int i = 0;
    // Search through our actor objects for dead game objects
    for (int i = 0; i < vActors.size(); i++) {
        temp = nullptr;

        // Our current actor object is dead
        if (vActors[i]->isDead()) {
            temp = vActors[i];
            vActors.erase(vActors.begin() + i);
            delete temp;

            // i-- so that we stay on the value we're currently on and don't iterate further
            //  or else we'll end up skipping objects which we need to delete. 
            i--;
        }
    }
}

// This function is used in tandem with the move() function and should thus only be called by said 
//  function 
bool StudentWorld::playerDiedDuringThisTick() {
    // Check to see if our player has either died or lost a life

    // If the player has hit escape then use this option
    if (getLives() != currLife) {
        // Update what life we're on
        currLife = getLives();

        return true;
    }
    // If the players health has dropped to or below zero
    else if (tMan->isAnnoyed()) {
        // Update what life we're on
        currLife = getLives();
        playSound(SOUND_PLAYER_GIVE_UP);

        return true;
    }

    // The player has not died
    return false;
}

void StudentWorld::placeGoldNuggets(int startX, int startY, int state, StudentWorld* tempWorld) {
    vActors.push_back(new GoldNugget(startX, startY, state, tempWorld));
}

// Utilized for digging in Tunnelman logic
void StudentWorld::digEarth(int x, int y) {
    // Check if there are any earth blocks within the Tunnelman objects hitbox (4x4 area)
    if (earthObjects[x][y] != nullptr  || earthObjects[x + 1][y + 1] != nullptr ||
        earthObjects[x + 2][y + 2] != nullptr || earthObjects[x + 3][y + 3] != nullptr) {
        // Dig out any blocks that Tunnelman touches with his hit box (4x4)
        for (int c = 0; c < 4; c++) {
            for (int r = 0; r < 4; r++) {
                if (earthObjects[x + c][y + r] != nullptr) {
                    delete earthObjects[x + c][y + r];
                    earthObjects[x + c][y + r] = nullptr;
                }
            }
        }

        playSound(SOUND_DIG);
    }

    /*
    B U G :
     - Move to X & Y = 60
     - Move down to Y = 3
     - Move left to X = 59
     - Move down to Y = 0
     - Now you will no longer be able to dig the right move block
    */
    // Bug Squashing code: 
    // Tunnelman is in the bottom right corner and there is still an earthObject there
    if (x + 3 == 63 and y == 0 && earthObjects[63][0] != nullptr) {
        // Delete any earthObjects that still exist within the given coordinates
        for (int r = 0; r < 4; r++) {
            delete earthObjects[63][r];
            earthObjects[63][r] = nullptr;
        }
        
        playSound(SOUND_DIG);
    }
}

// Utilized for Tunnelman inventory sonar ping
void StudentWorld::sonarPing(int x, int y) {
    // 
    for (int i = 0; i < vActors.size(); i++) {
        if (vActors[i]->objectType() == "GoldNugget" || vActors[i]->objectType() == "BarrelOfOil") {
            // Use distance formula to determine the distance(radius) the object is from us
            if (abs(vActors[i]->getX() - x) <= 12 && abs(vActors[i]->getY() - y) <= 12) {
                vActors[i]->setVisible(true);
            }
        }
    }
}

// Utilized by Boulder logic
void StudentWorld::removeEarth(int x, int y) {
    if (earthObjects[x][y] != nullptr) {
        delete earthObjects[x][y];
        earthObjects[x][y] = nullptr;
    }
}

bool StudentWorld::isEarth(int x, int y) {
    // Check four squares of earth
    for (int i = 0; i < 4; i++) {
        // Returns true if any of the one squares below are earth blocks
        if (earthObjects[x + i][y] != nullptr) {
            return true;
        }
    }

    // Returns false if all earth blocks below are no longer there
    return false;
}

bool StudentWorld::isBoulder(int x, int y) {
    for (int i = 0; i < vActors.size(); i++) {
        // If it is a boulder check the four squares
        if (vActors[i]->objectType() == "Boulder") {
            for (int j = 0; j < 4; j++) {
                int leftX = vActors[i]->getX() + j;
                // If it is a boulder with the same coordinates
                if (leftX == x && vActors[i]->getY() == y) {
                    return true;
                }

                int rightX = vActors[i]->getX() - j;
                // if it is a boulder with the same coordines
                if (rightX == x && vActors[i]->getY() == y) {
                    return true;
                }
            }
        }
    }

    return false;
}

int StudentWorld::getActorObjectX(string objectType) {
    if (objectType == tMan->objectType()) {
        return tMan->getX();
    }
}

int StudentWorld::getActorObjectY(string objectType) {
    if (objectType == tMan->objectType()) {
        return tMan->getY();
    }
}

// -1 is used as false in this case to show that nothing is in the calling objects hitbox
// Numbers from 0+ are considered either TRUE or can also be used for the position of said object in
//  calling objects hitbox, so that we may call to the object in our hitbox and properly interact with 
//  just that unique object. 
int StudentWorld::actorsInObjectHitBox(int x, int y, int xHitBox, int yHitBox, string objectType) {
    int vActorPos = -1;
    if (objectType == "Tunnelman") {
        if (abs(tMan->getX() - x) <= xHitBox && abs(tMan->getY() - y) <= yHitBox) {
            return 1;
        }
    }

    // Made specifically for protester
    // Will be called with xHitBox == 0 and yHitBox == 0 with boulder as obj type
    if (xHitBox == 0 && yHitBox == 0 && objectType == "Boulder") {
        for (int i = 0; i < vActors.size(); i++) {
            // If the calling object and the object we are searching for are within range
            if ((abs(x - vActors[i]->getX()) <= 4 && abs(y - vActors[i]->getY() <= 4) && vActors[i]->objectType() == "Boulder")) {
                return 0;
            }
        }
        return 1; //returns 1 if there is no earth or boulder
    }


    if (objectType == "Earth") {
        for (int i = 0; i < xHitBox; i++) {
            if (earthObjects[x + i][y + i] != nullptr) {
                vActorPos = 1;
            }
        }

        // There is an earth object in the calling objects hitbox
        return vActorPos;
    }

    else {
        for (int i = 0; i < vActors.size(); i++) {
            // Specific type case where boulder may find it's own self object and use that as a comparison
            //  leading to bugged code
            if (objectType == "Boulder" && (x == vActors[i]->getX() && y == vActors[i]->getY())) {
                continue;
            }

            // If the calling object and the object we are searching for are within range
            if ((abs(x - vActors[i]->getX()) <= xHitBox && abs(y - vActors[i]->getY() <= yHitBox)) && objectType == vActors[i]->objectType()) {
                vActorPos = i;
            }
        }
    }


    return vActorPos;
}

int StudentWorld::numActorObject(string objectType) {
    int count = 0;

    for (int i = 0; i < vActors.size(); i++) {
        // Count each time we find an object of type objectType 
        if (objectType == vActors[i]->objectType()/* && !vActors[i]->isDead()*/) {
            count++;
        }
    }

    return count;
}

// Update the inventory of the Tunnelman, this function is used for interaction between the Tunnelman's
//  inventory and the inventory items
void StudentWorld::inventoryUpdate(int item) {
    tMan->incrementInventoryCount(item);
}


void StudentWorld::dealDmg(int dmg, string objectType) {
    if (objectType == "Tunnelman") {
        tMan->setHitPoints(dmg);
    }
    //else if (objectType == "Protestor") {

    //}
}

void StudentWorld::updateDisplayText() {
    stringstream displayText;
    
    level = getLevel();
    lives = getLives();
    health = tMan->getHitPoints();
    squirts = tMan->getInventoryCount(1);
    gold = tMan->getInventoryCount(0);
    barrelsLeft = numActorObject("BarrelOfOil");
    sonar = tMan->getInventoryCount(2);
    score = getScore();


    // Format all text for display using string stream and iomanip
    // Reference : https://www.cplusplus.com/reference/sstream/stringstream/stringstream/ , Accessed 12/11/2021
    displayText << "Lvl: " << setw(2) << to_string(level);
    displayText << "  Lives: " << setw(1) << lives;
    displayText << " Hlth " << setw(3) << health * 10 << "%";
    displayText << "  Wtr: " << setw(2) << squirts;
    displayText << "  Gld: " << setw(2) << gold;
    displayText << "  Oil Left: " << setw(2) << barrelsLeft;
    displayText << "  Sonar: " << setw(2) << sonar;
    displayText << "  Scr: " << setfill('0') << setw(6) << score;

    //// The below code is for debugging purposes:
    //displayText << "X: " << tMan->getX();
    //displayText << " Y: " << tMan->getY();

    setGameStatText(displayText.str());
}

GameWorld* StudentWorld::getWorld(){
    return gWorld;
}

StudentWorld::~StudentWorld() {
    // Delete tunnelman object
    delete tMan;

    // Delete all remaining earth objects
    for (int c = 0; c < 64; c++) {
        for (int r = 0; r < 64; r++) {
            delete earthObjects[c][r];
        }
    }
}

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// SOURCE: https://stackoverflow.com/questions/7560114/random-number-c-in-some-range/7560151
// Range: [min, max]
int StudentWorld::random(int min, int max, char xOrY) {
    static bool first = true;
    int randNum = 0;

    if (first) {
        // Seeding for the first time only!
        srand(time(NULL));
        first = false;
    }

    // X-Coord: Ensure we don't spawn any objects in the tunnel shaft
    if (xOrY == 'X') {
        do {
            randNum = min + rand() % ((max + 1) - min);
        } while (randNum >= 27 && randNum <= 33);

        return randNum;
    }
    
    
    // Y-coord
    return min + rand() % ((max + 1) - min);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp