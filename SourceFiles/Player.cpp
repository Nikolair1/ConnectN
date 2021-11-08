// Player.cpp

#include "provided.h"
#include "support.h"
#include <string>
#include <iostream>
using namespace std;

class HumanPlayerImpl
{
public:
    int chooseMove(const Scaffold& s, int N, int color);
};

class BadPlayerImpl
{
public:
    int chooseMove(const Scaffold& s, int N, int color);
};

class SmartPlayerImpl
{
public:
    int chooseMove(const Scaffold& s, int N, int color);
};

int HumanPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    if (s.numberEmpty() == 0)
        return 0;
    int column=0;
    bool loop = true;
    string q="";
    while (loop) //set up a loop to validate the Human's move, keep reprompting until valid
    {
        if (color == 0)
            q = "Red";
        else
            q = "Black";
        cout << "Please enter a move, " << q << endl;
        cin >> column;
        int rows = s.levels();
        if (column > 0 && column <= s.cols())
        {
        for (int i = 1; i <= rows;i++)
        {
            if (s.checkerAt(column, i) == VACANT)
            {
                loop = false;
                break;
            }
        }
        }
    }
    return column;  
}

int BadPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    if (s.numberEmpty() == 0)
        return 0;
    
    int column = 1;
    int rows = s.levels();
  
    for (; column <= s.cols();column++) //the bad Player chooses the first column that has at least one vacant slot, from left to right
    {
        for (int i = 1; i <= rows;i++)
        {
            if (s.checkerAt(column, i) == VACANT)
            {
                return column;
            }

        }
    }
    return 0;
}




int SmartPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    Scaffold q(s.cols(), s.levels()); //Create a different scaffold
    
    q = s;                            //Copy s into the new scaffold
    set<pair<int, int>> collection;
   
    if (s.numberEmpty() == s.cols() * s.levels() && s.cols()>1) //makes sure that smart player chooses the middle as its first move
        return (s.cols() / 2)+1;
    

    pair<int, int> g = determineBestComputerMove(q, N, color, collection); 
    //calls recursive function to ^^^^
    //return a pair where the first value is the game outcome, and second outcome is the column
    
    return g.second;
}

//******************** Player derived class functions *************************

//  These functions simply delegate to the Impl classes' functions.
//  You probably don't want to change any of this code. 

HumanPlayer::HumanPlayer(string nm)
    : Player(nm)
{
    m_impl = new HumanPlayerImpl;
}

HumanPlayer::~HumanPlayer()
{
    delete m_impl;
}

int HumanPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

BadPlayer::BadPlayer(string nm)
    : Player(nm)
{
    m_impl = new BadPlayerImpl;
}

BadPlayer::~BadPlayer()
{
    delete m_impl;
}

int BadPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

SmartPlayer::SmartPlayer(string nm)
    : Player(nm)
{
    m_impl = new SmartPlayerImpl;
}

SmartPlayer::~SmartPlayer()
{
    delete m_impl;
}

int SmartPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}





