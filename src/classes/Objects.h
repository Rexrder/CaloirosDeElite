#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <iostream>
#include <ctime>
#include <cstring>
using std::time;

#define HEIGHT 900
#define WIDTH 1300

#ifndef SOUNDEFFECTS_H
#define SOUNDEFFECTS_H
struct SoundEffects
{
    ALLEGRO_SAMPLE *dead = NULL;
    ALLEGRO_SAMPLE *hit = NULL;
    ALLEGRO_SAMPLE *sp_att = NULL;
    ALLEGRO_SAMPLE *appear = NULL;
    ALLEGRO_SAMPLE *buff = NULL;
    ALLEGRO_SAMPLE *fast = NULL;
    ALLEGRO_SAMPLE *fortifyed = NULL;
};
#endif

#ifndef HIGHSCORESTAT_H
#define HIGHSCORESTAT_H
struct HighscrStat
{
    std::string player;
    int score;
};
#endif

#ifndef TIMERS_H
#define TIMERS_H
struct Timers
{
    std::time_t stunned = time(NULL);
    std::time_t invulnerable = time(NULL);
    std::time_t slowed = time(NULL);
    std::time_t buffed = time(NULL);
    std::time_t hacked = time(NULL);
    std::time_t fast = time(NULL);
    std::time_t fortifyed = time(NULL);
};

#endif

#ifndef STATE_H
#define STATE_H
struct State
{
    bool alive = true;
    bool fast = false;
    bool erase = false;
    bool stunned = false;
    bool invulnerable = false;
    bool slowed = false;
    bool buffed = false;
    bool hacked = false;
    bool fortifyed = false;
    Timers timer;
};

#endif

#ifndef OBJECTS_H
#define OBJECTS_H
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
#endif
