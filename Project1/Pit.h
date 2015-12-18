
#ifndef __Project_1__Pit__
#define __Project_1__Pit__

#include <stdio.h>
#include <string>
#include "globals.h"
#include "History.h"


class Player;
class Snake;

class Pit
{
public:
    // Constructor/destructor
    Pit(int nRows, int nCols);
    ~Pit();
    
    // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     snakeCount() const;
    int     numberOfSnakesAt(int r, int c) const;
    void    display(std::string msg) const;
    History&    history();
    
    // Mutators
    bool   addSnake(int r, int c);
    bool   addPlayer(int r, int c);
    bool   destroyOneSnake(int r, int c);
    bool   moveSnakes();
    
    
private:
    int         m_rows;
    int         m_cols;
    Player*     m_player;
    Snake*      m_snakes[MAXSNAKES];
    int         m_nSnakes;
    History     m_history;
};






#endif /* defined(__Project_1__Pit__) */
