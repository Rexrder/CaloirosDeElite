#include "Bullets.cpp"

class Player: public Objects
{
private:
    bool movState[2] = {false,false};
public:
    Player(int xstart, int ystart);
    ~Player();
    void move();
    void draw();
    void animate();
    Bullets* shoot();
    void changeMove(bool state, int pos);
};