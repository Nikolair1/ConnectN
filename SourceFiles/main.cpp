#include "provided.h"
#include "support.h"
#include <iostream>
#include <string>
#include <cassert>
#include <utility>
#include <set>

#if defined(_WIN32) || defined(_WIN64)
#include <iostream>
#include <windows.h>
#include <conio.h>

struct KeepWindowOpenUntilDismissed
{
    ~KeepWindowOpenUntilDismissed()
    {
        DWORD pids[1];
        if (GetConsoleProcessList(pids, 1) == 1)
        {
            std::cout << "Press any key to close this window . . . ";
            _getch();
        }
    }
} keepWindowOpenUntilDismissed;
#endif

using namespace std;

void doScaffoldTests()
{
    Scaffold s(3, 2);
    assert(s.cols() == 3 && s.levels() == 2 &&
        s.numberEmpty() == 6);
    assert(s.makeMove(1, RED));
    assert(s.makeMove(1, BLACK));
    assert(!s.makeMove(1, RED));
    assert(s.numberEmpty() == 4);
    assert(s.checkerAt(1, 1) == RED && s.checkerAt(1, 2) == BLACK);
    assert(s.checkerAt(2, 1) == VACANT);
    s.display();
}

void doPlayerTests()
{
    HumanPlayer hp("Marge");
    assert(hp.name() == "Marge" && hp.isInteractive());
    BadPlayer bp("Homer");
    assert(bp.name() == "Homer" && !bp.isInteractive());
    SmartPlayer sp("Lisa");
    assert(sp.name() == "Lisa" && !sp.isInteractive());
    Scaffold s(3, 2);
    s.makeMove(1, RED);
    s.makeMove(1, BLACK);
    cout << "=========" << endl;
    int n = hp.chooseMove(s, 3, RED);
    cout << "=========" << endl;
    assert(n == 2 || n == 3);
    n = bp.chooseMove(s, 3, RED);
    assert(n == 2 || n == 3);
    n = sp.chooseMove(s, 3, RED);
    assert(n == 2 || n == 3);
}

void doGameTests()
{
    BadPlayer bp1("Bart");
    BadPlayer bp2("Homer");
    Game g(2, 2, 2, &bp1, &bp2);
    int winner;
    assert(!g.completed(winner));
    g.takeTurn();  // Red's first move
    assert(!g.completed(winner) &&
        (g.checkerAt(1, 1) == RED || g.checkerAt(2, 1) == RED));
    g.takeTurn();  // Black's first move
    assert(!g.completed(winner));
    g.takeTurn();  // Red's second move; Red must win
    assert(g.completed(winner) && winner == RED);
}

int main()
{
    //doScaffoldTests();
   // doPlayerTests();
    //doGameTests();
    //cout << "Passed all tests" << endl;
    Scaffold s(3, 3);
    s.makeMove(1, RED);
    s.makeMove(1, RED);
    s.makeMove(1, BLACK);
    //s.makeMove(2, BLACK);
    //s.makeMove(2, BLACK);
   // s.makeMove(2, RED);
   // s.makeMove(2, BLACK);
    s.makeMove(3, BLACK);
    s.makeMove(3, RED);
    //s.makeMove(3, RED);
    //s.display();
   
    //cout << rating(s, 3);
    
    
   
    //s.display();
    //s.undoMove();
    //s.display();
   // cout<<s.numberEmpty();
   //pair<int, int> g(0, 0);
    //SmartPlayer sp("Ivan");
    //cout<<sp.chooseMove(s, 3, RED);
   //set<pair<int, int>> collection;
    //cout<<rating(s, 3, 2,BLACK);



    //g=determineBestComputerMove(s, 3, BLACK,collection,true);
    //cout << g.second;

    //for (auto it = collection.begin(); it !=
     //   collection.end(); ++it)
     //   cout << ' ' << (*it).first << " " <<(*it).second <<endl;
   
    //pair<int,int> p = determineBestComputerMove(s, 3,BLACK,collection);
    //cout << p.second;
    
    SmartPlayer bp("Homer");
    HumanPlayer hp("Marge");
    Game g(3, 4, 3, &hp, &bp);
   
     g.play();
   

    /*
    set<pair<int, int>> collection;
    pair<int, int> g(0, 0);
    pair<int, int> h(1, 0);
    pair<int, int> i(0, 1);
    pair<int, int> j(0, 1);
    collection.insert(g);
    collection.insert(h);
    collection.insert(i);
    
    cout << collection.size();
    */
}