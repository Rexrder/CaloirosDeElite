#include "Bullets.h"
#include "Objects.h"

#ifndef ENEMIES_H
#define ENEMIES_H
class Enemies: public Objects
{
private:
    bool moving_right = true;
public:
    Enemies(int xstart, int ystart, int type, double diff);
    ~Enemies();
    bool move(int n_left);
    void draw();
    void animate();
    void changeMove();
    void fortify();
    Bullets* shoot();
};

#endif