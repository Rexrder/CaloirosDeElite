#include "Objects.cpp"

class Bullets: public Objects
{
private:
    bool end = false;
public:
    Bullets(int xstart, int ystart, bool player);
    ~Bullets();
    bool isAlive();
    void move();
    void draw();
};