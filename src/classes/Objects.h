#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_windows.h>
#include <iostream>
#include <cmath>
#include <list>
#include "..\funcs\allfunc.h"
using std::cout;
using std::ceil;
using std::floor;

class Objects
{
protected:
    int x;
    int y;
    int size[2];
    int posSize[4];
    int speed;
    ALLEGRO_BITMAP *spritesheet;
public:
    Objects(int x = 0, int y = 0);
    ~Objects();
    int* getSize();
};
