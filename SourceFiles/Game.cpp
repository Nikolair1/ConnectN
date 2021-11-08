// Game.cpp

#include "provided.h"
#include "support.h"
#include <iostream>

using namespace std;
class GameImpl
{
public:
    GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black);
    bool completed(int& winner) const;
    bool takeTurn();
    void play();
    int checkerAt(int c, int r) const;
   
private:
    Scaffold s;
    Player* p1;
    Player* p2;
    int m_N;
    int whoJustWent = -1;
    int lastMove=-1;
};

GameImpl::GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black) : s(nColumns, nLevels), m_N(N)
{
    p1 = red;           //construct scaffold
    p2 = black;
}

bool GameImpl::completed(int& winner) const
{
    
    if (lastMove == -1)
        return false;
    int row=0;
    
    //this function knows the column of the last played move
    //in this for loop it finds the row of the last played move
    for (int i = 1; i <= s.levels();i++)
     {
        if (s.checkerAt(lastMove, i) == VACANT)
         {
                row=i-1;
                break;
         }
        else if (s.checkerAt(lastMove, s.levels()) == RED || s.checkerAt(lastMove, s.levels()) == BLACK)
        {
            row = s.levels();
            break;
        }

    }
    
    //check horizontally
    //BLack streak and Red Streak are used to keep track of a streak of pieces in a row
    int Count = 0;
    bool RedStreak = false;
    bool BlackStreak = false;
    for (int j = 1; j <= s.cols(); j++) //advance columns, keep level constant
    {
        if (s.checkerAt(j, row) == RED)
        {
            if (BlackStreak)
            {
                Count = 0;
                BlackStreak = false;
            }
            Count++;
            RedStreak = true;
        }
        else if (s.checkerAt(j, row) == BLACK)
        {
            if (RedStreak)
            {
                Count = 0;
                RedStreak = false;
            }
            Count++;
            BlackStreak = true;
        }
        else
        {
            Count = 0;
            RedStreak = false;
            BlackStreak = false;
        }

        if (Count >= m_N && BlackStreak)//if the streak count = N then we have a COnnect N
        {
            winner= BLACK;
            return true;
        }
        else if (Count >= m_N && RedStreak)
        {
           winner= RED;
           return true;
        }
    }

    //check vertically
    //now we advance the levels and look for streaks
    Count = 0;
    RedStreak = false;
    BlackStreak = false;
    for (int j = 1; j <= s.levels();j++)
    {
        if (s.checkerAt(lastMove, j) == RED)
        {
            if (BlackStreak)
            {
                Count = 0;
                BlackStreak = false;
            }
            Count++;
            RedStreak = true;
        }
        else if (s.checkerAt(lastMove, j) == BLACK)
        {
            if (RedStreak)
            {
                Count = 0;
                RedStreak = false;
            }
            Count++;
            BlackStreak = true;
        }
        else
        {
            Count = 0;
            RedStreak = false;
            BlackStreak = false;
        }


        if (Count >= m_N && BlackStreak)
        {

            winner= BLACK;
            return true;
        }
        else if (Count >= m_N && RedStreak)
        {

            winner= RED;
            return true;
        }
    }


    //check diagonally top left, bottom right
    //this part starts at the last played piece, and then goes up and to the left
    //as far as possible
    //Then it traverses down that diagonal until it reaches the bottom right
    //
    // |X| |   |
    // | |X|   |
    // | | | X |
    //
    //
    Count = 0;
    RedStreak = false;
    BlackStreak = false;
    int i = row;
    int j = lastMove;
    while (j>0&&i<=s.levels())  //first loop gets topmost part of the diagonal
    {
        
        j--;
        i++;
    }
    j++;
    i--;
    while (i>0 && j<=s.cols())
    {
        if (s.checkerAt(j, i) == RED)
        {
            if (BlackStreak)
            {
                Count = 0;
                BlackStreak = false;
            }
            Count++;
            RedStreak = true;
        }
        else if (s.checkerAt(j, i) == BLACK)
        {
            if (RedStreak)
            {
                Count = 0;
                RedStreak = false;
            }
            Count++;
            BlackStreak = true;
        }
        else
        {
            Count = 0;
            RedStreak = false;
            BlackStreak = false;
        }


        if (Count >= m_N && BlackStreak)
        {

            winner = BLACK;
            return true;
        }
        else if (Count >= m_N && RedStreak)
        {

            winner = RED;
            return true;
        }
        j++;
        i--;
    }



    //check diagonal top right,bottom left
     //this part starts at the last played piece, and then goes up and to the right
    //as far as possible
    //Then it traverses down that diagonal until it reaches the bottom left
    //
    // | | |X|
    // | |X| |
    // |X| | |
    //
    //
    Count = 0;
    RedStreak = false;
    BlackStreak = false;
    i = row;
    j = lastMove;
    while (j<=s.cols()&&i<=s.levels())  //first loop gets topmost part of the diagonal
    {

        j++;
        i++;
    }
    j--;
    i--;
    while (j>0&&i>0) //bounds checking
    {
        if (s.checkerAt(j, i) == RED)
        {
            if (BlackStreak)
            {
                Count = 0;
                BlackStreak = false;
            }
            Count++;
            RedStreak = true;
        }
        else if (s.checkerAt(j, i) == BLACK)
        {
            if (RedStreak)
            {
                Count = 0;
                RedStreak = false;
            }
            Count++;
            BlackStreak = true;
        }
        else
        {
            Count = 0;
            RedStreak = false;
            BlackStreak = false;
        }


        if (Count >= m_N && BlackStreak)
        {

            winner = BLACK;
            return true;
        }
        else if (Count >= m_N && RedStreak)
        {

            winner = RED;
            return true;
        }
        j--;
        i--;
    }

    if (s.numberEmpty() == 0) //finally, if no winner found and all slots are occupied, it's a tie
    {
        winner = TIE_GAME;
        return true;
    }

    return false;
}

bool GameImpl::takeTurn()
{
    int winner;
    if (this->completed(winner))
    {
        return false;
    }
    if (whoJustWent == -1) //whoJustWent is constructed as -1, indicating no player has gone in the game yet
    {
        int choice = p1->chooseMove(s, m_N, RED); //red should go first
        lastMove = choice;
        s.makeMove(choice, RED);
        whoJustWent = 0;                //markthatredwent
        return true;
    }
    else if (whoJustWent == 0) //if Red just went
    {
        int choice = p2->chooseMove(s, m_N, BLACK); //black's turn
        lastMove = choice;
        s.makeMove(choice, BLACK);
        whoJustWent = 1;
        return true;
    }
    else
    {
        int choice = p1->chooseMove(s, m_N, RED); //else, Black just went
        lastMove = choice;
        s.makeMove(choice, RED);
        whoJustWent = 0;
        return true;
    }
}

void GameImpl::play()
{
    int winner;
    string q;
    s.display();
    int counter = 1;
    if (!p1->isInteractive() && !p2->isInteractive()) //if both players are robots
    {
        while (!this->completed(winner))
        {
            cout << "Round: " << counter << endl; //display Round
            this->takeTurn();
            s.display();
            cout << "Press ENTER to continue"; //wait for spectator to press enter for the game to continue
            cin.ignore();
            counter++;
        }
    }
    else
    {
        while (!this->completed(winner))
        {
            cout << "Round: " << counter << endl;
            this->takeTurn();
            s.display();
            counter++;
        }
    }
    if (winner == -1)
    {
        cout<<"Tie Game!"<<endl;
        return;
    }
    if (winner == 0)
        q = "Red";
    else if (winner == 1)
        q = "Black";
    cout << endl << q << " is the winner!" << endl; //win message
}

int GameImpl::checkerAt(int c, int r) const
{
    int ret=s.checkerAt(c, r);
    if (ret == 0)
        return RED;
    else if (ret == 1)
        return BLACK;
    else
        return VACANT;
}

//******************** Game functions *******************************

//  These functions simply delegate to GameImpl's functions.
//  You probably don't want to change any of this code. 

Game::Game(int nColumns, int nLevels, int N, Player* red, Player* black)
{
    m_impl = new GameImpl(nColumns, nLevels, N, red, black);
}

Game::~Game()
{
    delete m_impl;
}

bool Game::completed(int& winner) const
{
    return m_impl->completed(winner);
}

bool Game::takeTurn()
{
    return m_impl->takeTurn();
}

void Game::play()
{
    m_impl->play();
}

int Game::checkerAt(int c, int r) const
{
    return m_impl->checkerAt(c, r);
}
