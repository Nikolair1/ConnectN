// Scaffold.cpp
#include <vector>
#include "provided.h"
#include <iostream>
#include <stack>

using namespace std;

class ScaffoldImpl
{
public:
    ScaffoldImpl(int nColumns, int nLevels);
    int cols() const;
    int levels() const;
    int numberEmpty() const;
    int checkerAt(int column, int level) const;
    void display() const;
    bool makeMove(int column, int color);
    int undoMove();
    
private:
    vector<vector<char>> grid;
    int m_cols;
    int m_levels;
    
    class Coord
    {
    public:
        Coord(int r, int c) : m_row(r), m_col(c) {}
        int r() const { return m_row; }
        int c() const { return m_col; }
    private:
        int m_row;
        int m_col;
    };
    stack<Coord> coordStack;

};

ScaffoldImpl::ScaffoldImpl(int nColumns, int nLevels)
{
    if (nColumns <= 0 || nLevels <= 0)
    {
        cerr << "No negative values!" << endl; exit(1);
    }
    else
    {
        m_cols = nColumns;
        m_levels = nLevels;
        grid.resize(m_levels+1);          //extra row 
        for (int i = 0; i < m_levels + 1; i++)
            grid[i].resize(2 * m_cols+1);  //2x+1 because of the wall characters

        
        for (int i = 0; i < m_levels + 1; i++)
            for (int j = 0; j < 2 * m_cols+1; j++)
                {
                if(i==m_levels)
                {
                    if(j%2==0)                  //bottom row has +-+-+-
                        grid[i][j] = '+';
                    else
                        grid[i][j] = '-';
                }
                else if(j%2==0)
                    grid[i][j] = '|';           //side has walls |    | 
                else
                {
                    grid[i][j] = ' ';
                }
                }

        

    }

}

int ScaffoldImpl::cols() const
{
    return m_cols;  
}

int ScaffoldImpl::levels() const
{

    return m_levels;  
}

int ScaffoldImpl::numberEmpty() const
{
    int counter=0;
    for (int i = 0; i < m_levels; i++)
        for (int j = 0; j < 2 * m_cols + 1; j++)
        {
            if (grid[i][j] == ' ')
                counter++;
        }
    
    return counter;
    
}

int ScaffoldImpl::checkerAt(int column, int level) const
{
    if (column <= m_cols && column > 0 && level <= m_levels && level > 0)
    {
        if (grid[m_levels - level][2 * column - 1] == 'R')
            return RED;
        else if (grid[m_levels - level][2 * column - 1] == 'B') //conversion to vector notation
            return BLACK;
        else
            return VACANT;
    }

    return VACANT; 
}

void ScaffoldImpl::display() const
{
    for (int i = 0; i < m_levels + 1; i++)
    { 
        for (int j = 0; j < 2 * m_cols + 1; j++)
        {
            cout << grid[i][j];
        }
        cout << endl;
    }
}

bool ScaffoldImpl::makeMove(int column, int color)
{
    bool test = false;
    int store = 0;
    if (column <= m_cols && column > 0 && (color==RED || color==BLACK))
    {
        for (int i = 0; i < m_levels; i++)
        {
            if (grid[i][2 * column - 1] == ' ')
            {
                test = true;
                store = i;
            }    
        }
        if (test)
        {
            if (color == RED)
                grid[store][2 * column - 1] = 'R';
            else
                grid[store][2 * column - 1] = 'B';
            
            Coord c(store, 2 * column - 1);
            coordStack.push(c);         //Push onto my stack of coordinates, makes it easy to find last move and keeps a history of moves
        }
        return test;
    }
    return false;  
}


int ScaffoldImpl::undoMove()
{
    if (m_cols * m_levels == this->numberEmpty())
        return 0;
   
    Coord b = coordStack.top();
    coordStack.pop();                   //pop off stack

    grid[b.r()][b.c()] = ' ';           //replace with blank char
    return (b.c() + 1) / 2;
}

//******************** Scaffold functions *******************************

//  These functions simply delegate to ScaffoldImpl's functions.
//  You probably don't want to change any of this code. 

Scaffold::Scaffold(int nColumns, int nLevels)
{
    m_impl = new ScaffoldImpl(nColumns, nLevels);
}

Scaffold::~Scaffold()
{
    delete m_impl;
}

Scaffold::Scaffold(const Scaffold& other)
{
    m_impl = new ScaffoldImpl(*other.m_impl);
}

Scaffold& Scaffold::operator=(const Scaffold& rhs)
{
    if (this != &rhs)
    {
        Scaffold temp(rhs);
        swap(m_impl, temp.m_impl);
    }
    return *this;
}

int Scaffold::cols() const
{
    return m_impl->cols();
}

int Scaffold::levels() const
{
    return m_impl->levels();
}

int Scaffold::numberEmpty() const
{
    return m_impl->numberEmpty();
}

int Scaffold::checkerAt(int column, int level) const
{
    return m_impl->checkerAt(column, level);
}

void Scaffold::display() const
{
    m_impl->display();
}

bool Scaffold::makeMove(int column, int color)
{
    return m_impl->makeMove(column, color);
}

int Scaffold::undoMove()
{
    return m_impl->undoMove();
}

