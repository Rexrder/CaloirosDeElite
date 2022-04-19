#include "Enemies.h"

Enemies::Enemies(int xstart, int ystart, int type, double diff) : Objects(xstart, ystart)
{
    size[0] = 64;
    size[1] = 64;

    ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_set_path_filename(path, pl_skins[type].c_str());
    spritesheet = al_load_bitmap(al_path_cstr(path, '/'));
    speed = 10 + ceil(10 * diff);
}

Enemies::~Enemies()
{
    al_destroy_bitmap(spritesheet);
}

Bullets *Enemies::shoot()
{
    return new Bullets(x + 16, y + 64, false);
}

void Enemies::animate()
{
    if (!state.alive){
        animation(5,7,&anim,&anim_mov,false);
    }
    else{
        if (lives == 2)
        {
            animation(8,10,&anim,&anim_mov,false);
        }
        else{
            animation(0,2,&anim,&anim_mov,true);   
        }
    }

    if (anim == 7){
        state.erase = true;
    }
}

void Enemies::fortify(){
    lives = 2;
}

bool Enemies::move(int n_left)
{
    if (moving_right)
    {
        x = ((ceil(speed / n_left)) > 30) ? x + 30 + ceil(speed / 10) : x + ceil(speed / n_left) + ceil(speed / 10);
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
    al_draw_bitmap_region(spritesheet, 64 * anim, 0, 64, 64, x, y, 0);
};