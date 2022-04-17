#include "Enemies.cpp"

class Bosses: public Objects
{
private:
    SoundEffects sounds;
    bool moving_right = true;
    int difficulty;
    int color[3];
    int type;
    int shot_gap = 0;
    float original_lives;
    bool mega_boss;
    ALLEGRO_FONT *font_boss;
public:
    Bosses(int, double, bool = false);
    ~Bosses();
    void move();
    void draw();
    void animate();
    void changeMove();
    int getType();
    int getShotGap();
    bool isMegaboss();
    Bullets* shootNormal();
    bool shootSpecial();
    void shot(int = 1, int = 0) override;
};