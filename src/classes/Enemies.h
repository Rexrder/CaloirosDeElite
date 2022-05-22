#include "Bullets.h"
#include "Objects.h"

#ifndef ENEMIES_H
#define ENEMIES_H
class Enemies: public Objects
{
private:
    bool moving_right = true;
    int type;
public:
    Enemies(int xstart, int ystart, int type, double diff, ALLEGRO_BITMAP* &spr);
    ~Enemies();
    bool move(int n_left);
    void draw();
    void animate();
    void changeMove();
    void fortify();
    Bullets* shoot();
};

#endif