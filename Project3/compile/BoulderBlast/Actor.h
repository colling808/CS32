#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor: public GraphObject
{
public:
    Actor(int imageID, int startX, int startY, Direction dir, StudentWorld* world);
    virtual ~Actor();
    virtual void doSomething();
    void setDead();
    virtual void getShot() = 0;
    
    virtual bool checkEmptySpace(int x, int y) const;
    bool checkBoulder(int x, int y, Actor* notMe) const;
    virtual bool blocksPlayer() const;
    virtual bool blocksRobot() const;
    virtual bool blocksBoulder() const;
    virtual bool blocksBullet() const;
    virtual bool blocksVision() const;
    virtual void moveObject(int deltaX, int deltaY, Direction dir);
    StudentWorld* getWorld() const;
    bool isAlive() const;
    virtual bool isShootable() const;
    virtual bool isStealable() const;
    
private:
    StudentWorld* m_world;
    bool m_alive;
};


class Player: public Actor
{
public:
    Player(int X, int Y, StudentWorld* world);       // constructor
    virtual ~Player();                              // destructor
    
    virtual void doSomething();
    virtual void moveObject(int deltaX, int deltaY, Direction dir);
    virtual void getShot();
    void shoot();
    
    void loadAmmo();
    void restoreHealth();
    void finishLevel();
    
    // Accessor Functions //
    int getHealth() const;
    int getAmmo() const;
    bool getVictory() const;

    
private:
    int m_health;
    int m_ammo;
    bool m_victory;
    
};


class Wall: public Actor
{
public:
    Wall(int X, int Y, StudentWorld* world);
    virtual ~Wall();
    virtual bool blocksBullet() const;
    virtual bool blocksVision() const;
    virtual bool isShootable() const;
    virtual void getShot() {};
private:
    
};


class Boulder: public Actor
{
public:
    Boulder(int X, int Y, StudentWorld* world);
    virtual ~Boulder();
    virtual void getShot();
    virtual void moveObject(int deltaX, int deltaY, Direction dir);
    virtual bool blocksPlayer() const;
    virtual bool blocksVision() const;
    virtual bool checkEmptySpace(int x, int y) const;
private:
    int m_health;
};


class Bullet: public Actor
{
public:
    Bullet(int X, int Y, Direction dir, StudentWorld* world);
    virtual ~Bullet();
    virtual bool isShootable() const;
    virtual bool blocksPlayer() const;
    virtual bool blocksBoulder() const;
    virtual bool blocksRobot() const;
    virtual void doSomething();
    virtual void getShot() {};
private:
    
};

class Hole: public Actor
{
public:
    Hole(int X, int Y, StudentWorld* world);
    virtual ~Hole();
    virtual void getShot(){}
    virtual void doSomething();
    virtual bool blocksBoulder() const;
    virtual bool isShootable() const;
private:
    
};


class Goodie: public Actor
{
public:
    Goodie(int image, int X, int Y, StudentWorld* world, int points);
    virtual ~Goodie();
    virtual void getShot() {};
    virtual bool isShootable() const;
    virtual bool blocksPlayer() const;
    virtual bool blocksRobot() const;
    virtual void doSomething();
    virtual void doExtra() = 0;
    void reveal();
    void hide();
    bool isRevealed() const;
private:
    int m_points;
    bool m_revealed;

    
};



class Jewel: public Goodie
{
public:
    Jewel(int X, int Y, StudentWorld* world);
    virtual ~Jewel();
    virtual void doExtra() {};
};


class Exit: public Goodie
{
public:
    Exit(int X, int Y, StudentWorld* world);
    virtual ~Exit();
    virtual void doSomething();
    virtual void doExtra(){};    
};

class ExtraLife: public Goodie
{
public:
    ExtraLife(int x, int y, StudentWorld* world);
    virtual ~ExtraLife();
    virtual void doExtra();
    virtual bool isStealable() const;
};


class RestoreHealth: public Goodie
{
public:
    RestoreHealth(int x, int y, StudentWorld* world);
    virtual ~RestoreHealth();
    virtual void doExtra();
    virtual bool isStealable() const;
};

class Ammo: public Goodie
{
public:
    Ammo(int x, int y, StudentWorld* world);
    virtual ~Ammo();
    virtual void doExtra();
    virtual bool isStealable() const;
};

class Robot: public Actor
{
public:
    Robot(int image, int x, int y, Direction dir, StudentWorld* world, int health, int points);
    virtual ~Robot();
    virtual void doSomething() = 0;
    virtual void getShot();
    void incTurn();
    virtual void moveObject(int deltaX, int deltaY, Direction dir);
    virtual bool checkEmptySpace(int x, int y) const;
    bool playerInSight() const;
    virtual bool blocksVision() const;
    int getTurn() const;
    int getTicks() const;
private:
    int m_health;
    int m_points;
    int m_ticks;
    int m_turn;
};

class Snarlbot: public Robot
{
public:
    Snarlbot(int x, int y, Direction dir, StudentWorld* world);
    virtual ~Snarlbot();
    virtual void doSomething();
};


class Kleptobot: public Robot
{
public:
    enum GoodieType { NONE, AMMO, RESTORE_HEALTH, EXTRA_LIFE};
    Kleptobot(int x, int y, StudentWorld* world, int imageID, int health, int points);
    virtual ~Kleptobot();
    virtual void doSomething();
    void pickUp(Goodie* ap);
    void drop();
    virtual bool isShootingRobot() const = 0;
private:
    int m_distBeforeTurn;
    int m_distTraveled;
    GoodieType m_goodieType;
};


class RegularKleptobot: public Kleptobot
{
public:
    RegularKleptobot(int x, int y, StudentWorld* world);
    virtual ~RegularKleptobot();
    virtual bool isShootingRobot() const;
};

class AngryKleptobot: public Kleptobot
{
public:
    AngryKleptobot(int x, int y, StudentWorld* world);
    virtual ~AngryKleptobot();
    virtual bool isShootingRobot() const;
};

class KleptobotFactory: public Actor
{
public:
    enum ProductType { REGULAR, ANGRY};
    KleptobotFactory(int x, int y, StudentWorld* world, ProductType type);
    virtual ~KleptobotFactory();
    virtual void doSomething();
    virtual void getShot(){};
    virtual bool isShootable() const;
    virtual bool blocksBullet() const;
    virtual bool blocksVision() const;
private:
    ProductType m_product;
    int m_count; 
};


#endif // ACTOR_H_
