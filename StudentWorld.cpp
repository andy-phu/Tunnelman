#include "StudentWorld.h"
#include <string>
#include <vector>
#include "Actor.h"  // We use this in the .cpp file instead of .h file due to wanting the actual implementation of Actor.h
                    //  classes, member functions and the like to be imported into this file for actual use. Due to coding
                    //  actual implementation of classes and the likes of StudentWorld.h in this file. -- Note line 9 in 
                    //  StudentWorld.h for further explanation. --
#include <memory>
#include <algorithm>


using namespace std;

StudentWorld::StudentWorld(string assetDir): GameWorld(assetDir){
    // this gives the address of this StudentWorld object
    gWorld = this;
    tMan = nullptr;
    //regPro = nullptr;
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
//    regPro = new Protestor(this, tMan);
//    vActors.push_back(regPro);;//first protestor added in
    level = getLevel();
    
    
    
    //push a pointer to an eart object into the vector in each array bucket
    //left of mine shaft in mid rows:0-29 cols:0-59
    for (int c = 0; c < 29; c++){
        for (int r = 0; r< 59; r++){
            earthObjects[c][r] = new Earth(c,r, this);
        }
    }
    
    //right of mine shaft
    for (int c = 34; c < 64; c++){
        for (int r = 0; r< 59; r++){
            earthObjects[c][r] = new Earth(c,r, this);
        }
    }
    
    //earth at the end of mine shaft
    for (int c = 29; c < 34; c++){
        for (int r = 0; r< 4; r++){
            earthObjects[c][r] = new Earth(c,r, this);
        }
    }
   
    
    //places the boulders
    int B = min(level/2 + 2, 9);
    int counter = 0;
    //adds the right amt of boulders for each level
    while(counter != B){
        int randX;
        do{
            randX = random(0,60); //0 to 60
        }while(randX == 27 || randX == 28 || randX == 29 || randX == 30 || randX == 31 || randX == 32 || randX == 33);

        int randY = random(20,56); //20 to 56

        cout << randX << " " << randY << endl;
        //places boulder
        boulder = new Boulder(randX, randY, this);
        //removes earth in a 4x4 square for boulder
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                removeEarth(randX+i, randY+j);
                removeEarth(randX+i, randY+j);
                removeEarth(randX+i, randY+j);
                removeEarth(randX+i, randY+j);
            }
        }
        vActors.push_back(boulder);
    counter++;
    }


    return GWSTATUS_CONTINUE_GAME;
}

//every tick this is called, thus gotta keep track of the amt of ticks that have occurred and reset once it hits a certain number to keep
//track when to add a protestor
int StudentWorld::move(){
    updateDisplayText();
    tMan->doSomething();
    
    //add protestors based on the ticks if the max protestors arent added yet
    int rightSideOfMin = 2 + level * 1.5;  //min doesn't like all at once
    int P = min(15, rightSideOfMin);
//    if(vActors.size() != P){
//        addProtester(current_level_number);
//    }
    
    for (int i = 0; i < vActors.size(); i++) {
            vActors[i]->doSomething();
        }
    
    //regPro->doSomething(); //might have to end up using a for loop and go through whole vector to call do something
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
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
    

}

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

    //test
    
    
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

void StudentWorld::removeEarth(int x, int y){
    if(earthObjects[x][y] != nullptr){
        delete earthObjects[x][y];
        earthObjects[x][y] = nullptr;
    }
}

void StudentWorld::updateDisplayText() {
    string displayText;

    //The below code is for debugging purposes:
    displayText = "X: ";
    displayText += to_string(tMan->getX());
    displayText += "\tY: ";
    displayText += to_string(tMan->getY());

    setGameStatText(displayText);
}

GameWorld* StudentWorld::getWorld(){
    return gWorld;
}

bool StudentWorld::isEarth(int x, int y){
    if(earthObjects[x][y] == nullptr){ //returns false when it is not equal to earth
        return false;
    }
    else{
        return true;
    }
}

bool StudentWorld::isBoulder(int x, int y){
    for(int i = 0; i < vActors.size(); i++){
        if(vActors[i]->objectType() == "Boulder" && vActors[i]->getX() == x && vActors[i]->getY() == y ){ //if it is a boulder with the same coordinates return true
            return true;
        }
    }
    return false;
}

//Tunnelman* StudentWorld::getTMan(){
//    return tMan;
//}

////might have to include the hard protestor rarity formula later
//void StudentWorld::addProtester(int levels){
//    int T = max(25, 200 - levels); //amt of ticks that must pass before adding a protestor
//    //int probabilityOfHardcore = min(90, current_level_number * 10 + 30); //add in later
//    //int rSideOfMin = 2 + levels * 1.5; //for some reason min doesn't like all this stuff at once
//    if(ticks == T){ //once the min amt of ticks has been achieved
//        regPro = new Protestor(this, tMan); //create new
//        vActors.push_back(regPro); //add into vector
//        ticks = 0; //reset
//    }
//    else{
//        ticks++; //increments ticks to keep track when to add a protestor
//    }
//}

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

//SOURCE:https://stackoverflow.com/questions/7560114/random-number-c-in-some-range/7560151
int StudentWorld::random(int min, int max) //range : [min, max]
{
   static bool first = true;
   if (first)
   {
      srand( time(NULL) ); //seeding for the first time only!
      first = false;
   }
   return min + rand() % (( max + 1 ) - min);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
