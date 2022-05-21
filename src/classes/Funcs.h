#include "Objects.h"
#ifndef FUNCS
#define FUNCS

bool verifyCollision(int*, int*);
bool animation(int start, int end, int&, int&, bool ping_pong);
bool compareHighs(const HighscrStat& first, const HighscrStat& second);
void rangeDef(int &val, int* rang);
#endif