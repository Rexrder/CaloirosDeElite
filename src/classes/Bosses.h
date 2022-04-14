#include "Enemies.cpp"

class Bosses: public Objects
{
private:
    bool moving_right = true;
    int difficulty;
    int color[3];
public:
    Bosses(int type, double diff);
    ~Bosses();
    void move();
    void draw();
    void animate();
    void changeMove();
    void kill();
    Bullets* shootNormal();
    bool shootSpecial();
    void shot(int = 1, int = 0) override;
};