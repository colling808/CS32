
#include "History.h"
#include "globals.h"
#include <iostream>

using namespace std;


History::History(int nRows, int nCols)
{
    m_rows = nRows;
    m_cols = nCols;
    
    for (int r = 0; r < m_rows; r++)
        for (int c = 0; c < m_cols; c++)
        {
            grid[r][c] = 0;
        }
    
}

void History::display() const
{
    {
        // Position (row,col) in the pit coordinate system is represented in
        // the array element grid[row-1][col-1]
        char displaygrid[MAXROWS][MAXCOLS];
        int r, c;
     
        // Fill the grid with dots
        for (r = 0; r < m_rows; r++)
            for (c = 0; c < m_cols; c++)
            {
                if (grid[r][c] >= 1)
                    displaygrid[r][c] = 'A' + (grid[r][c] - 1);
                else
                    displaygrid[r][c] = '.';
            }
        
        // Draw the grid
        clearScreen();
        for (r = 0; r < m_rows; r++)
        {
            for (c = 0; c < m_cols; c++)
                cout << displaygrid[r][c];
            cout << endl;
        }
        cout << endl;
      

    }
}


bool History::record (int r, int c)
{
    if (r < 1 || c < 1 || r > m_rows || c > m_cols)
        return false;
    else
        grid[r-1][c-1] += 1;
        return true;
}
