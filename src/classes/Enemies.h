#include "Player.cpp"

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
    void kill();
    Bullets* shoot();
};