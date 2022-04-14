#include "Bullets.cpp"

class Player: public Objects
{
private:
    bool movState[2] = {false,false}; //right and left movement
public:
    Player(int xstart, int ystart);
    ~Player();
    void move();
    void draw();
    void animate();
    Bullets* shoot();
    void changeMove(bool state, int pos);
    void shot(int = 1,int = 0) override;
};