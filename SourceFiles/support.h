
#ifndef support_h
#define support_h
#include <set>
#include "provided.h"
#include <iostream>

#include <utility>





std::pair<int, int> determineBestComputerMove(Scaffold& test, int m_N, int color, std::set<std::pair<int, int>>& collection);
std::pair<int, int> determineBestHumanMove(Scaffold& test, int m_N, int color, std::set<std::pair<int, int>>& collection);

int rating(const Scaffold& s, int m_N, int lastMove);



#endif

