
#ifndef __Project_1__History__
#define __Project_1__History__

#include <stdio.h>
#include "globals.h"


class History {
    
public:
    History(int nRows, int nCols);
    bool record (int r, int c);
    void display() const;
    
private:
    int     m_rows, m_cols;
    int     grid[MAXROWS][MAXCOLS];
    
};





#endif /* defined(__Project_1__History__) */
