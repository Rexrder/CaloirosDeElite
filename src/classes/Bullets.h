#include "Objects.cpp"

class Bullets: public Objects
{
public:
    Bullets(int xstart, int ystart, bool player, int type = 0);
    ~Bullets();
    void move();
    void draw();
    void collide();
};