#include "Objects.h"

#ifndef BULLETS_H
#define BULLETS_H

class Bullets: public Objects
{
public:
    Bullets(int xstart, int ystart, bool player, int type = 0);
    ~Bullets();
    void move();
    void draw();
    void collide();
};

#endif