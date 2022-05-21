#include "Bonus.h"
#include "Funcs.h"
#include <allegro5/allegro_image.h>
#include <allegro5/allegro5.h>
#include <cmath>

Bonus::Bonus(double diff)
{
    size[0] = 30;
    size[1] = 16;

    x = -10 - size[0];
    y = 300;

    ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_set_path_filename(path, "/res/coxotruck.png");
    spritesheet = al_load_bitmap(al_path_cstr(path, '/'));
    speed = 5 + std::ceil(1.5 * diff);
}

Bonus::~Bonus()
{
    al_destroy_bitmap(spritesheet);
}

void Bonus::animate()
{
    if (!state.alive)
    {
        animation(3, 5, anim, anim_mov, false);
    }
    else
    {
        animation(0, 2, anim, anim_mov, true);
    }
    if (anim == 5)
    {
        state.erase = true;
    }
}

void Bonus::move()
{
    x += speed;
}

void Bonus::draw()
{
    al_draw_bitmap_region(spritesheet, size[0] * anim, 0, size[0], size[1], x, y, 0);
};