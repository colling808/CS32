
#ifndef __Project_1__Game__
#define __Project_1__Game__

#include <stdio.h>

class Pit;

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nSnakes);
    ~Game();
    
    // Mutators
    void play();
    
private:
    Pit* m_pit;
};


#endif /* defined(__Project_1__Game__) */
