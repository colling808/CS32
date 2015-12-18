
#ifndef __Project_1__Snake__
#define __Project_1__Snake__

#include <stdio.h>

class Pit;

class Snake
{
public:
    // Constructor
    Snake(Pit* pp, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    
    // Mutators
    void move();
    
private:
    Pit* m_pit;
    int  m_row;
    int  m_col;
};



#endif /* defined(__Project_1__Snake__) */
