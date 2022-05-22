#include "Enemies.h"
#include "Funcs.h"
#include "Bullets.h"
#include <allegro5/allegro_image.h>
#include <allegro5/allegro5.h>
#include <cmath>

Enemies::Enemies(int xstart, int ystart, int type, double diff, ALLEGRO_BITMAP* &spr) : Objects(xstart, ystart)
{
    size[0] = 64;
    size[1] = 64;

    spritesheet = spr;
    speed = 10 + std::ceil(10 * diff);
    this->type = type;
}

Enemies::~Enemies()
{
}

Bullets *Enemies::shoot()
{
    return new Bullets(x + 16, y + 64, false);
}

void Enemies::animate()
{
    if (!state.alive)
    {
        animation(5, 7, anim, anim_mov, false);
    }
    else
    {
        if (lives == 2)
        {
            animation(8, 10, anim, anim_mov, false);
        }
        else
        {
            animation(0, 2, anim, anim_mov, true);
        }
    }

    if (anim == 7)
    {
        state.erase = true;
    }
}

void Enemies::fortify()
{
    lives = 2;
}

bool Enemies::move(int n_left)
{
    if (moving_right)
    {
        x = ((std::ceil(speed / n_left)) > 30) ? x + 30 + ceil(speed / 10) : x + ceil(speed / n_left) + ceil(speed / 10);
        if (x > WIDTH - size[0] - 60)
        {
            return true;
        }
    }
    else
    {
        x = ((ceil(speed / n_left)) > 30) ? x - (30 + ceil(speed / 10)) : x - (ceil(speed / n_left) + ceil(speed / 10));
        if (x < 60)
        {
            return true;
        }
    }
    return false;
}

void Enemies::changeMove()
{
    y += 74;
    moving_right = !moving_right;
}

void Enemies::draw()
{
    al_draw_bitmap_region(spritesheet, 64 * anim, 320 - 64*type, 64, 64, x, y, 0);
};