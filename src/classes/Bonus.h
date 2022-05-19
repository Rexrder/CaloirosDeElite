#include "Objects.h"

#ifndef BONUS_H
#define BONUS_H
class Bonus: public Objects
{
public:
    Bonus(double diff);
    ~Bonus();
    void move();
    void draw();
    void animate();
};

#endif