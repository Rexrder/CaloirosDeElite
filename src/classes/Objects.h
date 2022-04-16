#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_windows.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <iostream>
#include <cmath>
#include <ctime>
#include <list>
#include "..\funcs\allfunc.h"
using std::ceil;
using std::cout;
using std::floor;
using std::time;

struct SoundEffects
{
    ALLEGRO_SAMPLE *dead = NULL;
    ALLEGRO_SAMPLE *hit = NULL;
    ALLEGRO_SAMPLE *sp_att = NULL;
    ALLEGRO_SAMPLE *appear = NULL;
    //ALLEGRO_SAMPLE *buff = NULL;
};
struct Timers
{
    std::time_t stunned = time(NULL);
    std::time_t invulnerable = time(NULL);
    std::time_t slowed = time(NULL);
    std::time_t buffed = time(NULL);
    std::time_t hacked = time(NULL);
};
struct State
{
    bool alive = true;
    bool erase = false;
    bool stunned = false;
    bool invulnerable = false;
    bool slowed = false;
    bool buffed = false;
    bool hacked = false;
    Timers timer;
};
class Objects
{
protected:
    State state;
    int x;
    int y;
    int size[2];
    int speed;
    int anim = 1;
    int anim_mov = 1;
    int lives = 1;
    int posSize[4] = {x, y, size[0], size[1]};
    ALLEGRO_BITMAP *spritesheet;

public:
    Objects(int x = 0, int y = 0);
    ~Objects();
    State getState();
    int *getSize();
    virtual void shot(int = 1, int = 0);
    void updateState();
};
