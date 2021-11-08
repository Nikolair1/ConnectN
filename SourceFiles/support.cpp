#include "support.h"
#include <iostream>
#include <set>
#include <utility>
using namespace std;

pair<int, int> determineBestComputerMove(Scaffold& test, int m_N, int color, set<pair<int, int>>& collection)
{

    int rows = test.levels();

    pair<int, int> g(-10, -10);

    for (int column = 1; column <= test.cols();column++) //Computer iterates through all possible moves
    {
        for (int i = 1; i <= rows;i++)
        {
            if (test.checkerAt(column, i) == VACANT && (i-1==0 || test.checkerAt(column, i - 1) == 1 || test.checkerAt(column, i - 1) == 0))
            {                                           //if it finds a valid move, i.e a VACANT spot that is not floating
                test.makeMove(column, color);           //it makes the move
                int value = rating(test, m_N, column);  //then stores the rating of the scaffold at this time

               //the rating function returns a high positive number when BLACK wins, and low negative number when RED wins, 0 for a tie, or 
               //2 if the game isn't over

                if ((color == BLACK && value > 100) || (color == RED && value < -100) || value == 0)
                {
                    g.first = value;    //if the move resulted in a win or tie, store it in the set of pairs
                    g.second = column;
                    collection.insert(g);

                }
                else
                {
                    if (color == RED)
                        g = determineBestHumanMove(test, m_N, BLACK, collection); //else call determineBestHumanMove
                    else
                        g = determineBestHumanMove(test, m_N, RED, collection);

                    collection.insert(g);  //and add its value to the set of pairs
                }
                test.undoMove();  //finally, undo the move
            }

        }
    }
    if (color == BLACK)
    {
        auto itr = collection.end();  //BLACK wins will be stored at the end of the set, Sets are ordered numerically from least to greatest
        return *(--itr);
    }
    else
    {
        auto itr = collection.begin(); //RED wins will be stored at the start of the set
        return *itr;
    }
}


pair<int, int> determineBestHumanMove(Scaffold& test, int m_N, int color, set<pair<int, int>>& collection)
{

    int rows = test.levels();

    pair<int, int> g(-10, -10);




    for (int column = 1; column <= test.cols();column++)  //determineBestHumanMove function will iterate through all possible moves that the computer can make
    {
        for (int i = 1; i <= rows;i++)
        {
            if (test.checkerAt(column, i) == VACANT && (i-1 == 0 || test.checkerAt(column, i - 1) == 1 || test.checkerAt(column, i - 1) == 0))
            {
                test.makeMove(column, color);
                int value = rating(test, m_N, column);
              
                if ((color == BLACK && value > 100) || (color == RED && value < -100) || value == 0)    
                {
                    g.first = -1*value;  //If it detects a win, it will store the NEGATIVE of the rating function
                    g.second = column;
                    collection.insert(g);
                   
                }
                else
                {
                    if (color == RED)
                        g = determineBestComputerMove(test, m_N, BLACK, collection); //if no win or tie, call determineBestComputerMove
                    else
                        g = determineBestComputerMove(test, m_N, RED, collection);

                    collection.insert(g);
                }
                test.undoMove();
            }

        }
    }
    if (color == BLACK)
    {
        auto itr = collection.end();
        return *(--itr);
    }
    else
    {
        auto itr = collection.begin();
        return *itr;
        
    }
}

int rating(const Scaffold& s, int m_N, int lastMove)
{

    int row = 0;

    //this function's logic is identical to Game::Complete
    //except the output is an int instead of bool

    for (int i = 1; i <= s.levels();i++)
    {
        if (s.checkerAt(lastMove, i) == VACANT)
        {
            row = i - 1;
            break;
        }
        else if (s.checkerAt(lastMove, s.levels()) == RED || s.checkerAt(lastMove, s.levels()) == BLACK)
        {
            row = s.levels();
            break;
        }

    }

    //check horizontally

    int Count = 0;
    bool RedStreak = false;
    bool BlackStreak = false;
    for (int j = 1; j <= s.cols(); j++)
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

        if (Count >= m_N && BlackStreak)
        { //depth is calculated using the number of pieces currently on the board
            return 500 - (s.cols() * s.levels() - s.numberEmpty()); //if BLACK wins, take 500 and subtract the depth
        }
        else if (Count >= m_N && RedStreak)
        {
            return -500 + (s.cols() * s.levels() - s.numberEmpty()); //if RED wins, take -500 and add the depth
        }
    }

    //check vertically
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

            return 500 - (s.cols() * s.levels() - s.numberEmpty());
        }
        else if (Count >= m_N && RedStreak)
        {
            return -500 + (s.cols() * s.levels() - s.numberEmpty());

        }
    }


    //check diagonally top left, bottom right
    Count = 0;
    RedStreak = false;
    BlackStreak = false;
    int i = row;
    int j = lastMove;
    while (j > 0 && i <= s.levels())  //first loop gets topmost part of the diagonal
    {

        j--;
        i++;
    }
    j++;
    i--;
    while (i > 0 && j <= s.cols())
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
            return 500 - (s.cols() * s.levels() - s.numberEmpty());
        }
        else if (Count >= m_N && RedStreak)
        {

            return -500 + (s.cols() * s.levels() - s.numberEmpty());
        }
        j++;
        i--;
    }



    //check diagonal top right,bottom left
    Count = 0;
    RedStreak = false;
    BlackStreak = false;
    i = row;
    j = lastMove;
    while (j <= s.cols() && i <= s.levels())  //first loop gets topmost part of the diagonal
    {

        j++;
        i++;
    }
    j--;
    i--;
    while (j > 0 && i > 0)
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

            return 500 - (s.cols() * s.levels() - s.numberEmpty());
        }
        else if (Count >= m_N && RedStreak)
        {

            return -500 + (s.cols() * s.levels() - s.numberEmpty());
        }
        j--;
        i--;
    }

    if (s.numberEmpty() == 0)
    {
        return 0; //return 0 for a tie
    }

    return 2; //return 2 if game isn't over
}




