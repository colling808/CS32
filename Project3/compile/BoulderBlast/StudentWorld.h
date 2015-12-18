#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include "Actor.h"
#include <list>
#include <string>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(string assetDir);           // constructor
    virtual ~StudentWorld();                // deconstructor
    
    
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    virtual void setDisplayText();
    unsigned int decBonus();
    bool fireUpon(int x, int y);
    bool doFactoryCensus(int x, int y, int distance, int& count, Actor* notMe);
    void makeBullet(int x, int y, GraphObject::Direction dir);
    void makeGoodie(int x, int y, Kleptobot::GoodieType type);
    void makeKleptobot(int x, int y, KleptobotFactory::ProductType type);
    void exposeExitIfReady();
    bool canPlayerMoveHere(int x, int y) const;
    bool canRobotMoveHere(int x, int y) const;
    bool canBoulderMoveHere(int x, int y) const;
    bool doesSpaceBlockVision(int x, int y) const;
    
    // Accessor Functions //
    
    unsigned int getBonus() const;
    Actor* getActorAt(int x, int y, Actor* notMe) const;
    Player* getPlayer() const;
    Actor* getStealable(int x, int y) const;

private:
    list <Actor*> m_actors;
    Player* m_player;
    unsigned int m_bonus;
    
};

#endif // STUDENTWORLD_H_
