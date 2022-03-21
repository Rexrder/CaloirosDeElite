#include "Player.h"

Player::Player(int xstart, int ystart) : Objects(xstart, ystart)
{
    ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_set_path_filename(path, "/res/eletrao.png");
    spritesheet = al_load_bitmap(al_path_cstr(path, '/'));
    speed = 15;

    size[0] = 64;
    size[1] = 64;
}

Player::~Player()
{
    al_destroy_bitmap(spritesheet);
}

void Player::changeMove(bool state, int pos){
    movState[pos] = state;
}

Bullets* Player::shoot(){
    return new Bullets(x+20,y-28, true);
    }

void Player::animate(){
    anim += anim_mov;
    if (anim != 1){
        anim_mov = -anim_mov;
    }
}

void Player::move(){
    if (movState[0]){
        x -= speed;
    }
    if (movState[1]){
        x += speed;
    }
    x = (x < 1026) ? x : x - speed;
    x = (x > 10) ? x : x + speed;
}

void Player::draw(){
    al_draw_bitmap_region(spritesheet, 64*anim, 0, 64, 64, x, y, 0);
};