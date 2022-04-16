#include "Enemies.cpp"

class Bosses: public Objects
{
private:
    SoundEffects sounds;
    bool moving_right = true;
    int difficulty;
    int color[3];
    int type;
public:
    Bosses(int type, double diff);
    ~Bosses();
    void move();
    void draw();
    void animate();
    void changeMove();
    int getType();
    Bullets* shootNormal();
    bool shootSpecial();
    void shot(int = 1, int = 0) override;
};