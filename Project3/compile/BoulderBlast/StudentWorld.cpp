#include "StudentWorld.h"
#include <string>
#include <list>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp


StudentWorld::StudentWorld(string assetDir)           // constructor
: GameWorld(assetDir)
{
    m_player = nullptr;
    
}




StudentWorld::~StudentWorld()
{
    delete m_player;
    m_player = nullptr;
    
    list<Actor*>:: iterator itr = m_actors.begin();
    
    while(!m_actors.empty())
    {
        list<Actor*>:: iterator killMe = itr;
        delete *killMe;
        m_actors.erase(killMe);
    }
    
}





int StudentWorld::init()
{
 
    ostringstream oss;
    
    oss.fill('0');
    oss << "level" << setw(2) << getLevel() << ".dat";
    
    
    string curLevel = oss.str();
    Level lev(assetDirectory());
    Level::LoadResult result = lev.loadLevel(curLevel);
    
    if(getLevel() > 99 || result == Level::load_fail_file_not_found)
        return GWSTATUS_PLAYER_WON;
    
    if(result == Level::load_fail_bad_format)
        return GWSTATUS_LEVEL_ERROR;
    
    m_bonus = 1000;
    
    for(int x = 0; x < VIEW_WIDTH; x++)
    {
        for(int y = 0; y < VIEW_HEIGHT; y++)
        {
            Level::MazeEntry item = lev.getContentsOf(x, y);
            switch(item)
            {
                case Level::player:
                    m_player = new Player(x,y, this);
                    break;
                case Level::wall:
                    m_actors.push_back(new Wall(x,y, this));
                    break;
                case Level::boulder:
                    m_actors.push_back(new Boulder(x,y, this));
                    break;
                case Level::hole:
                    m_actors.push_back(new Hole(x, y, this));
                    break;
                case Level::jewel:
                    m_actors.push_back(new Jewel(x, y, this));
                    break;
                case Level::extra_life:
                    m_actors.push_back(new ExtraLife(x,y,this));
                    break;
                case Level::restore_health:
                    m_actors.push_back(new RestoreHealth(x,y,this));
                    break;
                case Level::ammo:
                    m_actors.push_back(new Ammo(x,y,this));
                    break;
                case Level::exit:
                    m_actors.push_back(new Exit(x,y,this));
                    break;
                case Level::vert_snarlbot:
                    m_actors.push_back(new Snarlbot(x,y,GraphObject::down,this));
                    break;
                case Level::horiz_snarlbot:
                    m_actors.push_back(new Snarlbot(x,y,GraphObject::right,this));
                    break;
                case Level::kleptobot_factory:
                    m_actors.push_back(new KleptobotFactory(x, y, this, KleptobotFactory::REGULAR));
                    break;
                case Level::angry_kleptobot_factory:
                    m_actors.push_back(new KleptobotFactory(x, y, this, KleptobotFactory::ANGRY));
                default:
                    break;
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}


int StudentWorld::move()
{
    
    setDisplayText();
    
    list<Actor*>::iterator itr = m_actors.begin();
    while(itr != m_actors.end())
    {
        if((*itr)->isAlive())
        {
            (*itr)->doSomething();
         
            if(!m_player->isAlive())
            {
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
            
            if(m_player->getVictory())
                return GWSTATUS_FINISHED_LEVEL;
        }
        itr++;
    }
    
    
    if(m_player->isAlive())
    {
        m_player->doSomething();
        
        if(!m_player->isAlive())
        {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        
        if(m_player->getVictory())
            return GWSTATUS_FINISHED_LEVEL;
    }
    
    // removes dead game actors
    itr = m_actors.begin();
    while(itr != m_actors.end())
    {
        if(!(*itr)->isAlive())
        {
            list<Actor*>:: iterator killMe = itr;
            delete *killMe;
            m_actors.erase(killMe);
            //itr = m_actors.begin();
        }
        itr++;
    }
    

    decBonus();
    
    exposeExitIfReady();
    
    
    if(!m_player->isAlive())
    {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
    if(m_player->getVictory())
        return GWSTATUS_FINISHED_LEVEL;
    
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete m_player;
    m_player = nullptr;
    
    list<Actor*>:: iterator itr = m_actors.begin();
    
    while(!m_actors.empty())
    {
        list<Actor*>:: iterator killMe = itr;
        delete *killMe;
        m_actors.erase(killMe);
        //itr = m_actors.begin();
        
        itr++;
    }
}

void StudentWorld::setDisplayText()
{
    int score = getScore();
    int level = getLevel();
    unsigned int bonus = getBonus();
    int livesLeft = getLives();
    int health = m_player->getHealth() * 5;
    int ammo = m_player->getAmmo();
    
    ostringstream oss;
    
    oss.fill('0');
    oss << "Score: " << setw(7) << score << "  Level: " << setw(2) << level;
    
    oss.fill(' ');
    oss << "  Lives: " << setw(2) << livesLeft << "  Health: " << setw(3) << health << "%  Ammo: " << setw(3) << ammo << "  Bonus: " << setw(4) << bonus << endl;
    
    string s = oss.str();
    setGameStatText(s);
    
}

void StudentWorld::makeBullet(int x, int y, GraphObject::Direction dir)
{
    int deltaX=0, deltaY=0;
    switch(dir)
    {
        case GraphObject::right:
            deltaX = 1; break;
        case GraphObject::left:
            deltaX = -1; break;
        case GraphObject::down:
            deltaY = -1; break;
        case GraphObject::up:
            deltaY = 1; break;
        default: break;
    }
    
    m_actors.push_back(new Bullet(x+deltaX, y+deltaY, dir, this));
    
}

void StudentWorld::makeGoodie(int x, int y, Kleptobot::GoodieType type)
{
    switch(type)
    {
        case Kleptobot::NONE:
            return;
        case Kleptobot::AMMO:
            m_actors.push_back(new Ammo(x, y, this));
            break;
        case Kleptobot::RESTORE_HEALTH:
            m_actors.push_back(new RestoreHealth(x, y, this));
            break;
        case Kleptobot::EXTRA_LIFE:
            m_actors.push_back(new ExtraLife(x, y, this));
            break;
    }
    
    
}

void StudentWorld::makeKleptobot(int x, int y, KleptobotFactory::ProductType type)
{
    if(type == KleptobotFactory::REGULAR)
        m_actors.push_back(new RegularKleptobot(x, y, this));
    
    else
        m_actors.push_back(new AngryKleptobot(x,y, this));
    
}

bool StudentWorld::doFactoryCensus(int x, int y, int distance, int& count, Actor* notMe)
{
    
    int c = 0;
    Actor* ap = nullptr;
    Kleptobot* kp = nullptr;
    {
    for(int i = x - distance; i <= x + distance; i++)
        for(int j = y - distance; j <= x + distance; j++)
        {
            list<Actor*>::iterator itr = m_actors.end();
            itr--;
            while (itr != m_actors.begin())
            {
                if((*itr)->getX() == i && (*itr)->getY() == j && (*itr) != notMe)
                {
                    kp = dynamic_cast<Kleptobot*>(*itr);
                    if(kp!= nullptr)
                    {
                        ap = *itr;
                        break;
                    }
                }
                itr--;
            }

            if(kp == nullptr)
                continue;
            kp = dynamic_cast<Kleptobot*>(ap);
            if(kp != nullptr)
                c++;
            
            if(kp->getX() == x && kp->getY() == y)
                return false;
        }
    }
    
    count = c;
    return true;
}

bool StudentWorld::fireUpon(int x, int y)
{
    if(m_player->getX() == x && m_player->getY() == y)
    {
        m_player->getShot();
        return true;
    }
    
    
    list<Actor*>::iterator itr = m_actors.end();
    itr--;
    
    while (itr != m_actors.begin())
    {
        if((*itr)->getX() == x && (*itr)->getY() == y)
        {
            if((*itr)->isShootable())
            {
                (*itr)->getShot();
                return true;
            }
            
            if((*itr)->blocksBullet())
                return true;
        }
        
        itr--;
    }
    return false;
}





void StudentWorld::exposeExitIfReady()
{
    list<Actor*>::iterator itr = m_actors.begin();
    int count = 0;
    
    while(itr != m_actors.end())
    {
        Jewel* jp = dynamic_cast<Jewel*>((*itr));
        if(jp != nullptr)
            count++;
        itr++;
    }
    
   if(count <= 0)
    {
        itr = m_actors.begin();
        Exit* ep = nullptr;
        
        while(itr != m_actors.end())
        {
            ep = dynamic_cast<Exit*>((*itr));
            if(ep != nullptr)
                break;
            itr++;
        }
        
        if(!ep->isRevealed())
        {
            ep->setVisible(true);
            ep->reveal();
            playSound(SOUND_REVEAL_EXIT);
        }
    }
}



unsigned int StudentWorld::decBonus()
{
    if(m_bonus == 0)
        return 0;
    
    return m_bonus--;
}



unsigned int StudentWorld::getBonus() const
{
    return m_bonus;
}


Player* StudentWorld::getPlayer() const
{
    return m_player;
}


Actor* StudentWorld::getActorAt(int x, int y, Actor* notMe) const
{
    
    list<Actor*> v = m_actors;
    list<Actor*>::iterator itr = v.begin();
    
    while (itr != v.end())
    {
        if((*itr)->getX() == x && (*itr)->getY() == y && (*itr) != notMe)
            return *itr;
        itr++;
    }
    return nullptr;
}

bool StudentWorld::canPlayerMoveHere(int x, int y) const
{
    list<Actor*> v = m_actors;
    list<Actor*>:: iterator itr = v.begin();
    
    while(itr!= v.end())
    {
        if((*itr)->getX() == x && (*itr)->getY() == y && (*itr)->blocksPlayer())
            return true;
        itr++;
    }
    
    return false;
}

bool StudentWorld::canRobotMoveHere(int x, int y) const
{
    list<Actor*> v = m_actors;
    list<Actor*>:: iterator itr = v.begin();
    
    if(m_player->getX() == x && m_player->getY() == y)
        return true;
    
    while(itr!= v.end())
    {
        if((*itr)->getX() == x && (*itr)->getY() == y && (*itr)->blocksRobot())
            return true;
        itr++;
    }
    
    return false;
}





bool StudentWorld::canBoulderMoveHere(int x, int y) const
{
    list<Actor*> v = m_actors;
    list<Actor*>:: iterator itr = v.begin();
    
    while(itr!= v.end())
    {
        if((*itr)->getX() == x && (*itr)->getY() == y && (*itr)->blocksBoulder())
            return true;
        itr++;
    }
    
    return false;
}

bool StudentWorld::doesSpaceBlockVision(int x, int y) const
{
    list<Actor*> v = m_actors;
    list<Actor*>:: iterator itr = v.begin();
    
    while(itr!= v.end())
    {
        if((*itr)->getX() == x && (*itr)->getY() == y && (*itr)->blocksVision())
            return true;
        itr++;
    }
    
    return false;
}


Actor* StudentWorld::getStealable(int x, int y) const
{
    list<Actor*> v = m_actors;
    list<Actor*>:: iterator itr = v.begin();
    
    while(itr!= v.end())
    {
        if((*itr)->getX() == x && (*itr)->getY() == y && (*itr)->isStealable())
            return (*itr);
        itr++;
    }
    return nullptr;
}









