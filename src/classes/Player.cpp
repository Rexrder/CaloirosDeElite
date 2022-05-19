#include "Player.h"
#include "Funcs.h"
#include "Bullets.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

Player::Player(int pl_type, int max_l)
{
    ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    type = pl_type;
    al_set_path_filename(path, pl_skins[type].c_str());
    spritesheet = al_load_bitmap(al_path_cstr(path, '/'));

    path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_set_path_filename(path, "/res/life.png");
    spritesheet_life = al_load_bitmap(al_path_cstr(path, '/'));

    path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_set_path_filename(path, "/res/sound/effects/pldead.ogg");
    sounds.dead = al_load_sample(al_path_cstr(path, '/'));

    path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_set_path_filename(path, "/res/sound/effects/plshot.wav");
    sounds.hit = al_load_sample(al_path_cstr(path, '/'));

    path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_set_path_filename(path, "/res/sound/effects/buff.ogg");
    sounds.buff = al_load_sample(al_path_cstr(path, '/'));

    speed = 15;
    lives = (max_l > 3) ? 3 : max_l;
    size[0] = 64;
    size[1] = 64;

    max_lives = max_l;
    x = WIDTH/2 - size[0]/2;
    y = HEIGHT - 70;
}

Player::~Player()
{
    al_destroy_bitmap(spritesheet);
    al_destroy_sample(sounds.hit);
    al_destroy_sample(sounds.dead);
}

void Player::changeMove(bool state, int pos){
    movState[pos] = state;
}

Bullets* Player::shoot(){
    return new Bullets(x+20,y-28, true);
    }

void Player::animate(){

    if (!state.alive){
        Funcs::animation(5,7,&anim,&anim_mov,false);
    }
    else if (state.fast)
    {
        Funcs::animation(11,13,&anim,&anim_mov,true);
    }
    else if (state.invulnerable){
        Funcs::animation(3,4,&anim,&anim_mov,true);
    }
    else
    if (state.fortifyed)
    {
        Funcs::animation(8,10,&anim,&anim_mov,true);
    }
    else if (state.buffed)
    {
        Funcs::animation(14,16,&anim,&anim_mov,true);
    }
    else{
        Funcs::animation(0,2,&anim,&anim_mov,true);
    }

    if (anim == 7){
        state.erase = true;
    }

    Funcs::animation(3,5,&lif_anim,&lif_anim_mov,true);

    if(last_lives - lives != 0){
        if((lives - last_lives) > 0){
            if (Funcs::animation(0,2,&lif_new_anim,&lif_new_anim_mov,false))
            {
                last_lives = lives;
            }
        }
        else{
            if (Funcs::animation(6,8,&lif_new_anim,&lif_new_anim_mov,false))
            {
                last_lives = lives;
            }
        }
    }
}

void Player::move(){
    int realSpeed = (state.fast) ? speed*2 : speed;
    realSpeed = (state.slowed) ? realSpeed/2 : realSpeed;

    y += y_speed;

    state.erase = (y+size[1] < 0) ? true : state.erase;

    updateState();
    if (state.buffed){
        damage = 5;
    }else
    {
        damage = 1;
    }
    

    if (movState[0]){
        x -= realSpeed;
    }
    if (movState[1]){
        x += realSpeed;
    }
    x = (x < WIDTH - size[0] - 60) ? x : x - realSpeed;
    x = (x > 60) ? x : x + realSpeed;
}

void Player::shot(int lost, int type){
    if (!state.fortifyed){
        Objects::shot(lost,type);
    }
    state.fortifyed = false;

    if (lives > 0){
        state.invulnerable = true;
        state.timer.invulnerable = time(NULL) + 3; //3 seconds of invulnerability
        al_play_sample(sounds.hit, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    else{
        al_play_sample(sounds.dead, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    lif_new_anim = 5;
}

void Player::buff(int type){
    al_play_sample(sounds.buff, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    switch (type)
    {
    case 0:
        state.invulnerable = true;
        state.timer.invulnerable = time(0) + 10;
        state.fast = true;
        state.timer.fast = time(0) + 10;
        break;
    
    case 1:
        state.invulnerable = true;
        state.timer.invulnerable = time(0) + 5;
        lives++;
        lif_new_anim = 0;
        break;

    case 2:
        state.buffed = true;
        state.timer.buffed = time(0) + 10;
        break;
    }
    if(lives > max_lives){
        state.fortifyed = true;
        state.timer.fortifyed = time(0) + 15;
        lives = max_lives;
    }
}

int Player::getType(){
    return type;
}

int Player::getDamage(){
    return damage;
}

void Player::win(){
    y_speed = (y_speed > -15) ? y_speed - 1 : -15;
}

void Player::draw(){
    int design_lives = (lives < last_lives) ? lives : last_lives;
    al_draw_bitmap_region(spritesheet, 64*anim, 0, 64, 64, x, y, 0);
    for (int i = 1; i <= design_lives; i++){
        al_draw_bitmap_region(spritesheet_life, 64*lif_anim,0,64,64,0,HEIGHT-(i*64),0);
    }
    for (int i = design_lives+1; i <= design_lives + std::abs(lives - last_lives); i++){
        al_draw_bitmap_region(spritesheet_life, 64*lif_new_anim,0,64,64,0,HEIGHT-(i*64),0);
    }
};