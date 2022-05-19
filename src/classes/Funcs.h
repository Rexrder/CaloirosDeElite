#ifndef FUNCS_H
#define FUNCS_H

class Funcs
{
public:
    Funcs(/* args */);
    ~Funcs();
    static bool verifyCollision(int*, int*);
    static bool animation(int start, int end, int*, int*, bool ping_pong);
};

#endif