#include "Enemies.h"

Enemies::Enemies(int xstart, int ystart, int type, double diff) : Objects(xstart, ystart)
{
    size[0] = 64;
    size[1] = 64;

    ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    switch (type)
    {
    case 0:
        al_set_path_filename(path, "/res/egsi.png");
        break;
    
    case 1:
        al_set_path_filename(path, "/res/epol.png");
        break;

    case 2:
        al_set_path_filename(path, "/res/eciv.png");
        break;
    }
    spritesheet = al_load_bitmap(al_path_cstr(path, '/'));
    speed = 10 + ceil(10*diff);
}

Enemies::~Enemies()
{
    al_destroy_bitmap(spritesheet);
}

Bullets* Enemies::shoot(){
    return new Bullets(x+16,y+64, false);
    }

void Enemies::animate(){
    anim += anim_mov;
    if (anim != 1){
        anim_mov = -anim_mov;
    }
}

bool Enemies::move(int n_left){
    if (moving_right){
        x = ((ceil(speed/n_left))>30) ? x + 30 + ceil(speed/10): x + ceil(speed/n_left) + ceil(speed/10);
        if (x>1120){
            return true;
        }
    }
    else{
        x = ((ceil(speed/n_left))>30) ? x - (30 + ceil(speed/10)): x - (ceil(speed/n_left) + ceil(speed/10));
        if (x<60){
            return true;
        }
    }
    return false;    
}

void Enemies::changeMove(){
    y += 74;
    moving_right = !moving_right;
}

void Enemies::draw(){
    al_draw_bitmap_region(spritesheet, 64*anim, 0, 64, 64, x, y, 0);
};