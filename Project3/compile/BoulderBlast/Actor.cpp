
#include "Actor.h"
#include "StudentWorld.h"

// ACTOR //

Actor::Actor(int imageID, int startX, int startY, Direction dir, StudentWorld* world)
:GraphObject(imageID, startX, startY, dir)
{
    setVisible(true);
    m_world = world;
    m_alive = true;
}

Actor::~Actor()
{
}

void Actor::doSomething()
{
    // does nothing/
}

void Actor::setDead()
{
    m_alive = false;
}


bool Actor::checkBoulder(int x, int y, Actor* notMe) const
{

    Actor *ap = getWorld()->getActorAt(x, y, notMe);
    
    Boulder *bp = dynamic_cast<Boulder*>(ap);
    if(bp != nullptr)
        return true;
    
    return false;
    
}

void Actor::moveObject(int deltaX, int deltaY, Direction dir)
{
    switch(dir)
    {
        case right:
            deltaX = 1; break;
        case left:
            deltaX = -1; break;
        case down:
            deltaY = -1; break;
        case up:
            deltaY = 1; break;
        default:
            return;
    }
    
    moveTo(getX()+deltaX, getY()+deltaY);
}





bool Actor::checkEmptySpace(int x, int y) const
{
    if(getWorld()->canPlayerMoveHere(x, y))
        return false;
    return true;
}


bool Actor::blocksPlayer() const
{
    return true;
}

bool Actor::blocksRobot() const
{
    return true;
}


bool Actor::blocksBoulder() const
{
    return true;
}

bool Actor::blocksBullet() const
{
    return false;
}

bool Actor::blocksVision() const
{
    return false;
}

bool Actor::isShootable() const
{
    return true;
}

bool Actor::isAlive() const
{
    return m_alive;
}

bool Actor::isStealable() const
{
    return false;
}


StudentWorld* Actor::getWorld() const
{
    return m_world;
}





// PLAYER //

Player::Player(int X, int Y, StudentWorld* world)
:Actor(IID_PLAYER, X, Y, right, world)
{
    m_health = 20;
    m_ammo = 20;
    m_victory = false;

}


Player::~Player()
{
}

void Player::doSomething()
{
    if(!isAlive())
        return;
    
    int ch;
    
    if(getWorld()->getKey(ch))
    {
        switch(ch)
        {
            case KEY_PRESS_ESCAPE:
                setDead();
                break;
            case KEY_PRESS_SPACE:
                shoot();
                break;
            case KEY_PRESS_LEFT:
                moveObject(-1, 0, left);
                break;
            case KEY_PRESS_RIGHT:
                moveObject(1, 0, right);
                break;
            case KEY_PRESS_DOWN:
                moveObject(0, -1, down);
                break;
            case KEY_PRESS_UP:
                moveObject(0, 1, up);
                break;
            default:
                break;
        }
    }
}

void Player::moveObject(int deltaX, int deltaY, Direction dir)
{
    setDirection(dir);
    if(checkEmptySpace(getX() + deltaX, getY() + deltaY))
    {
        Actor *ap = getWorld()->getActorAt(getX() + deltaX, getY() + deltaY, nullptr);
        if(checkBoulder(getX() + deltaX, getY() + deltaY, this))
        {
            ap->moveObject(deltaX, deltaY, dir);
            if(ap->getX() == getX()+deltaX && ap->getY() == getY()+deltaY)
                return;
        }
        moveTo(getX() + deltaX, getY() + deltaY);
    }
    
}


void Player::getShot()
{
    m_health -= 2;
    
    if(m_health > 0)
    {
        getWorld()->playSound(SOUND_PLAYER_IMPACT);
        return;
    }

    getWorld()->playSound(SOUND_PLAYER_DIE);
    setDead();
    
}


void Player::shoot()
{
    if(getAmmo() <= 0)
        return;
    
    m_ammo--;
    
    getWorld()->makeBullet(getX(), getY(), getDirection());
    getWorld()->playSound(SOUND_PLAYER_FIRE);
    
}

void Player::restoreHealth()
{
    m_health = 20;
}

void Player::loadAmmo()
{
    m_ammo += 20;
}

void Player::finishLevel()
{
    m_victory = true;
}

int Player::getHealth() const
{
    return m_health;
}

int Player::getAmmo() const
{
    return m_ammo;
}

bool Player::getVictory() const
{
    return m_victory;
}

// WALL //

Wall::Wall(int X, int Y, StudentWorld* world)
:Actor(IID_WALL, X, Y, none, world)
{
}

Wall::~Wall()
{
}

bool Wall::blocksBullet() const
{
    return true;
}

bool Wall::blocksVision() const
{
    return true;
}

bool Wall::isShootable() const
{
    return false;
}


// BOULDER //

Boulder::Boulder(int X, int Y, StudentWorld* world)
:Actor(IID_BOULDER, X, Y, none, world)
{
    m_health = 10;
}

Boulder::~Boulder()
{
}

void Boulder::moveObject(int deltaX, int deltaY, Direction dir)
{
    if(checkEmptySpace(getX()+deltaX, getY()+deltaY))
       moveTo(getX()+deltaX, getY()+deltaY);
}

void Boulder::getShot()
{
    m_health -= 2;
    if(m_health <= 0)
        setDead();
}


bool Boulder::checkEmptySpace(int x, int y) const
{
    if(getWorld()->canBoulderMoveHere(x, y))
        return false;
    return true;
}


bool Boulder::blocksPlayer() const
{
    return false;
}

bool Boulder::blocksVision() const
{
    return true;
}



// BULLET //


Bullet::Bullet(int x, int y, Direction dir, StudentWorld* world)
:Actor(IID_BULLET, x, y, dir, world)
{
}

Bullet::~Bullet()
{
}

void Bullet::doSomething()
{
    if(!isAlive())
        return;
    
    if(getWorld()->fireUpon(getX(), getY()))
        setDead();
    moveObject(0, 0, getDirection());
    
    if(!isAlive())
        return;
    
    if(getWorld()->fireUpon(getX(), getY()))
        setDead();

}

bool Bullet::isShootable() const
{
    return false;
}


bool Bullet::blocksPlayer() const
{
    return false;
}

bool Bullet::blocksBoulder() const
{
    return false;
}

bool Bullet::blocksRobot() const
{
    return false;
}
// HOLE //

Hole::Hole(int x, int y, StudentWorld* world)
:Actor(IID_HOLE, x, y, none, world)
{
}

Hole::~Hole()
{
}

void Hole::doSomething()
{
    if(!isAlive())
        return;
    
    if(checkBoulder(getX(), getY(), this))
    {
        setDead();
        getWorld()->getActorAt(getX(), getY(), this)->setDead();
    }
}

bool Hole::blocksBoulder() const
{
    return false;
}

bool Hole::isShootable() const
{
    return false;
}

// GOODIE //

Goodie::Goodie(int image, int x, int y, StudentWorld* world, int points)
:Actor(image, x, y,none, world)
{
    m_points = points;
    reveal();
}

Goodie::~Goodie()
{
}

void Goodie::doSomething()
{
    if(!isAlive())
        return;
    
    Player* pp = getWorld()->getPlayer();
    if(pp->getX() == getX() && pp->getY() == getY() && isRevealed())
    {
        getWorld()->increaseScore(m_points);
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        doExtra();
    }
}

void Goodie::reveal()
{
    m_revealed = true;
    setVisible(true);
}

void Goodie::hide()
{
    m_revealed = false;
    setVisible(false);
}

bool Goodie::isRevealed() const
{
    return m_revealed;
}

bool Goodie::isShootable() const
{
    return false;
}

bool Goodie::blocksPlayer() const
{
    return false;
}
bool Goodie::blocksRobot() const
{
    return false;
}

// JEWEL //


Jewel::Jewel(int x, int y, StudentWorld* world)
:Goodie(IID_JEWEL, x, y, world, 50)
{
}

Jewel::~Jewel()
{
}

// EXIT //

Exit::Exit(int x, int y, StudentWorld* world)
:Goodie(IID_EXIT, x, y, world, 2000)
{
    hide();
}

Exit::~Exit()
{
}

void Exit::doSomething()
{
    Player* pp = getWorld()->getPlayer();
    if(pp->getX() == getX() && pp->getY() == getY() && isRevealed())
    {
        getWorld()->playSound(SOUND_FINISHED_LEVEL);
        getWorld()->increaseScore(2000 + getWorld()->getBonus());
        pp->finishLevel();
    }
}


// EXTRA LIFE GOODIE //

ExtraLife::ExtraLife(int x, int y, StudentWorld* world)
:Goodie(IID_EXTRA_LIFE, x, y, world, 1000)
{
}

ExtraLife::~ExtraLife()
{
}

void ExtraLife::doExtra()
{
    getWorld()->incLives();
}

bool ExtraLife::isStealable() const
{
    return true;
}


// RESTORE HEALTH GOODIE //


RestoreHealth::RestoreHealth(int x, int y, StudentWorld* world)
:Goodie(IID_RESTORE_HEALTH, x, y, world, 500)
{
}
RestoreHealth::~RestoreHealth()
{
}
void RestoreHealth::doExtra()
{
    getWorld()->getPlayer()->restoreHealth();
}

bool RestoreHealth::isStealable() const
{
    return true;
}

// AMMO //

Ammo::Ammo(int x, int y, StudentWorld* world)
:Goodie(IID_AMMO,x, y, world, 100)
{
}
Ammo::~Ammo()
{
}
void Ammo::doExtra()
{
    getWorld()->getPlayer()->loadAmmo();
}

bool Ammo::isStealable() const
{
    return true;
}

// ROBOT //

Robot::Robot(int image, int x, int y, Direction dir, StudentWorld* world, int health, int points)
:Actor(image, x, y, dir, world)
{
    m_health = health;
    m_points = points;
    m_turn = 0;
    
    m_ticks = (28 - getWorld()->getLevel()) / 4;
    if(m_ticks < 3)
        m_ticks = 3;
}

Robot::~Robot()
{
}

void Robot::getShot()
{
    m_health -= 2;
    
    if(m_health > 0)
    {
        getWorld()->playSound(SOUND_ROBOT_IMPACT);
        return;
    }
    
    setDead();
    getWorld()->playSound(SOUND_ROBOT_DIE);
    getWorld()->increaseScore(m_points);
    
}

void Robot::incTurn()
{
    m_turn++;
}

bool Robot::playerInSight() const
{
    Player* pp = getWorld()->getPlayer();
    Direction dir = getDirection();
    
    switch(dir)
    {
        case right:
            if(getY() == pp->getY() && getX() < pp->getX())
            {
                for(int k = getX()+1; k < pp->getX(); k++)
                {
                    if(getWorld()->doesSpaceBlockVision(k, getY()))
                       return false;
                }
            return true;
            } break;
        case left:
            if(getY() == pp->getY() && getX() > pp->getX())
            {
                for(int k = getX()-1; k > pp->getX(); k--)
                    if(getWorld()->doesSpaceBlockVision(k, getY()))
                        return false;
                return true;
            } break;
        case down:
            if(getX() == pp->getX() && getY() > pp->getY())
            {
                for(int k = getY()-1; k > pp->getY(); k--)
                {
                    if(getWorld()->doesSpaceBlockVision(getX(), k))
                        return false;
                }
                return true;
            } break;
        case up:
            if(getX() == pp->getX() && getY() < pp->getY())
            {
                for(int k = getY()+1; k < pp->getY(); k++)
                {
                    if(getWorld()->doesSpaceBlockVision(getX(), k))
                        return false;
                }
                return true;
            } break;
        default:
            break;
    }
    return false;
    
}

void Robot::moveObject(int deltaX, int deltaY, Direction dir)
{
    switch(dir)
    {
        case right:
            deltaX = 1; break;
        case left:
            deltaX = -1; break;
        case down:
            deltaY = -1; break;
        case up:
            deltaY = 1; break;
        default:
            return;
    }
    
    
    if(checkEmptySpace(getX()+deltaX, getY()+deltaY))
        moveTo(getX()+deltaX, getY()+deltaY);
    
    else
    switch(dir)
    {
        case right:
            setDirection(left);
            break;
        case left:
            setDirection(right);
            break;
        case up:
            setDirection(down);
            break;
        case down:
            setDirection(up);
            break;
        default:
            break;
    }
}

bool Robot::checkEmptySpace(int x, int y) const
{
    if(!getWorld()->canRobotMoveHere(x, y))
        return true;
    return false;
}


bool Robot::blocksVision() const
{
    return true;
}


int Robot::getTurn() const
{
    return m_turn;
}

int Robot::getTicks() const
{
    return m_ticks;
}


// SNARLBOT //


Snarlbot::Snarlbot(int x, int y, Direction dir, StudentWorld* world)
:Robot(IID_SNARLBOT, x, y, dir, world, 10, 100)
{
}
Snarlbot::~Snarlbot()
{
}
void Snarlbot::doSomething()
{
    incTurn();
    
    if(!isAlive() || getTurn() % getTicks()!= 0)
        return;

    if(playerInSight())
    {
        getWorld()->makeBullet(getX(), getY(), getDirection());
        getWorld()->playSound(SOUND_ENEMY_FIRE);
        return;
    }

    moveObject(0,0,getDirection());
    
}


// KLEPTOBOT //

Kleptobot::Kleptobot(int x, int y, StudentWorld* world, int imageID, int hp, int points)
:Robot(imageID, x, y, right, world, hp, points)
{
    m_distBeforeTurn = rand() % 6 + 1;
    m_goodieType = NONE;
    m_distTraveled = 0;
}


Kleptobot::~Kleptobot()
{
    drop();
}

void Kleptobot::doSomething()
{
    incTurn();
    
    if(!isAlive() || getTurn() % getTicks()!= 0)
        return;
    
    if(isShootingRobot() && playerInSight())
    {
        getWorld()->makeBullet(getX(), getY(), getDirection());
        getWorld()->playSound(SOUND_ENEMY_FIRE);
        return;
    }
    
    if(getWorld()->getStealable(getX(), getY()) != nullptr && rand() % 9 == 0)
    {
        Goodie*gp = dynamic_cast<Goodie*>(getWorld()->getStealable(getX(), getY()));
        pickUp(gp);
        getWorld()->playSound(SOUND_ROBOT_MUNCH);
        return;
    }
    
    int deltaX = 0, deltaY = 0;
    switch(getDirection())
    {
        case right:
            deltaX = 1; break;
        case left:
            deltaX = -1; break;
        case down:
            deltaY = -1; break;
        case up:
            deltaY = 1; break;
        default:
            return;
    }
    if(m_distTraveled < m_distBeforeTurn && checkEmptySpace(getX() + deltaX, getY() + deltaY))
    {
        
        moveTo(getX() + deltaX, getY() + deltaY);
        m_distTraveled++;
        return;
    }

    m_distBeforeTurn = rand() % 6 + 1;
    m_distTraveled = 0;
    int currentX = getX(), currentY = getY(), dir = rand() % 4, counter = 0;
    
    while(getX() == currentX && getY() == currentY)
    {
    
        deltaX = 0, deltaY = 0;
        
        if(counter > 3)
            break;
        
        switch(dir % 4)
        {
            case 0:
                deltaX = 1; break;
            case 1:
                deltaX = -1; break;
            case 2:
                deltaY = -1; break;
            case 3:
                deltaY = 1; break;
        }
        
        if(checkEmptySpace(getX()+deltaX, getY()+deltaY))
        {
            switch(dir % 4)
            {
                case 0:
                    setDirection(right);
                    break;
                case 1:
                    setDirection(left);
                    break;
                case 2:
                    setDirection(down);
                    break;
                case 3:
                    setDirection(up);
                    break;
            }
            moveTo(getX()+deltaX, getY()+deltaY);
            m_distTraveled++;
            return;
        }
        counter++;
        dir++;
    }
    
    switch(dir % 4)
    {
        case 0:
            setDirection(right);
            break;
        case 1:
            setDirection(left);
            break;
        case 2:
            setDirection(down);
            break;
        case 3:
            setDirection(up);
            break;
    }
}


void Kleptobot::pickUp(Goodie* gp)
{
    if(m_goodieType != NONE)
        return;

    Ammo* ap = dynamic_cast<Ammo*> (gp);
    RestoreHealth* rp = dynamic_cast<RestoreHealth*>(gp);
    ExtraLife* ep = dynamic_cast<ExtraLife*>(gp);
    
    if(ap != nullptr)
    {
        m_goodieType = AMMO;
        ap->setDead();
        return;
    }
    
    if(rp != nullptr)
    {
        m_goodieType = RESTORE_HEALTH;
        rp->setDead();
        return;
    }
    
    if(ep != nullptr)
    {
        m_goodieType = EXTRA_LIFE;
        ep->setDead();
        return;
    }

    
}

void Kleptobot::drop()
{
    getWorld()->makeGoodie(getX(), getY(), m_goodieType);
}


// Regular Kleptobot //

RegularKleptobot::RegularKleptobot(int x, int y, StudentWorld* world)
:Kleptobot(x, y, world, IID_KLEPTOBOT, 5, 10)
{
}

RegularKleptobot::~RegularKleptobot()
{
}

bool RegularKleptobot::isShootingRobot()  const
{
    return false;
}


// Angry Kleptobot //

AngryKleptobot::AngryKleptobot(int x, int y, StudentWorld* world)
:Kleptobot(x, y, world, IID_ANGRY_KLEPTOBOT, 8, 20)
{
}

AngryKleptobot::~AngryKleptobot()
{
}

bool AngryKleptobot::isShootingRobot() const
{
    return true;
}

// Kleptobot Factory //

KleptobotFactory::KleptobotFactory(int x, int y, StudentWorld* world, ProductType type)
:Actor(IID_ROBOT_FACTORY, x, y, none, world)
{
    m_product = type;
    m_count = 0;
}

KleptobotFactory::~KleptobotFactory()
{
}

void KleptobotFactory::doSomething()
{
    
    Actor* ap = getWorld()->getActorAt(getX(), getY(), this);
    
    if(ap != nullptr)       // there is a robot on factory already, do not make another one
        return;
    
    if(getWorld()->doFactoryCensus(getX(), getY(), 3, m_count, this))
        if(m_count < 3 && rand() % 50 == 48)
           {
               getWorld()->makeKleptobot(getX(), getY(), m_product);
               getWorld()->playSound(SOUND_ROBOT_BORN);
           }
    
}

bool KleptobotFactory::isShootable() const
{
    return false;
}

bool KleptobotFactory::blocksBullet() const
{
    return true;
}

bool KleptobotFactory::blocksVision() const
{
    return true;
}












