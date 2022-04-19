#include "Bullets.cpp"

class Player: public Objects
{
private:
    SoundEffects sounds;
    bool movState[2] = {false,false}; //right and left movement
    int type;
    int lif_anim = 3;
    int lif_anim_mov = 1;
    int lif_new_anim = 3;
    int lif_new_anim_mov = 1;
    int last_lives = 0;
    ALLEGRO_BITMAP *spritesheet_life;
public:
    Player(int = 0);
    ~Player();
    void move();
    void draw();
    void animate();
    void buff(int);
    int getType();
    Bullets* shoot();
    void changeMove(bool state, int pos);
    void shot(int = 1,int = 0) override;
};