#include "Player.cpp"

class Enemies: public Objects
{
private:
    int anim = 0;
    int anim_mov = 1;
    bool moving_right = true;
public:
    Enemies(int xstart, int ystart, int type, double diff);
    ~Enemies();
    bool move(int n_left);
    void draw();
    void animate();
    void changeMove();
    Bullets* shoot();
};