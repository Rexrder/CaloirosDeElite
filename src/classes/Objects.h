#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_windows.h>
#include <iostream>
#include <cmath>
#include <ctime>
#include <list>
#include "..\funcs\allfunc.h"
using std::cout;
using std::ceil;
using std::floor;
using std::time;

class Objects
{
protected:
    int x;
    int y;
    int size[2];
    int speed;
    int anim = 1;
    int anim_mov = 1;
    int lives = 1;
    bool aliveErase[2] = {true,false};
    int posSize[4] = {x,y,size[0],size[1]};
    ALLEGRO_BITMAP *spritesheet;
public:
    Objects(int x = 0, int y = 0);
    ~Objects();
    bool* isAlive();
    int* getSize();
    void shot(int);
};
