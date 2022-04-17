#include "Objects.cpp"

class Bullets: public Objects
{
private:
    bool end;
public:
    Bullets(int xstart, int ystart, bool player, int type = 0);
    ~Bullets();
    bool isAlive();
    void move();
    void draw();
    void collide();
};