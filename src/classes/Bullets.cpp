#include "Bullets.h"

Bullets::Bullets(int xstart, int ystart,bool player) : Objects(xstart, ystart)
{
    speed = -15;
    if (!player){
        speed = -speed;
    }

    size[0] = 12;
    size[1] = 24;

    ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    spritesheet = al_create_bitmap(32, 32);
    al_set_path_filename(path, "/res/bullets.png");
    spritesheet = al_load_bitmap(al_path_cstr(path, '/'));
}

Bullets::~Bullets()
{
    al_destroy_bitmap(spritesheet);
}

void Bullets::move(){
    y += speed;
    if (y <= 5 || y >= 1100){
        end = true;
    }
}

void Bullets::draw(){
    al_draw_bitmap(spritesheet, x, y, 0);
};

bool Bullets::isAlive(){
    return end;
}