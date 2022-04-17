#include "Player.h"

Player::Player(int xstart, int ystart) : Objects(xstart, ystart)
{
    ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_set_path_filename(path, "/res/eletrao.png");
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
    lives = 3;
    size[0] = 64;
    size[1] = 64;
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
        animation(5,7,&anim,&anim_mov,false);
    }else
    if (state.invulnerable){
        animation(3,4,&anim,&anim_mov,true);
    }
    else{
        animation(0,2,&anim,&anim_mov,true);
    }

    if (anim == 7){
        state.erase = true;
    }

    animation(3,5,&lif_anim,&lif_anim_mov,true);

    if(last_lives - lives != 0){
        if((lives - last_lives) > 0){
            if (animation(0,2,&lif_new_anim,&lif_new_anim_mov,false))
            {
                last_lives = lives;
            }
        }
        else{
            if (animation(6,8,&lif_new_anim,&lif_new_anim_mov,false))
            {
                last_lives = lives;
            }
        }
    }
}

void Player::move(){
    int realSpeed = (state.buffed) ? speed*2 : speed;
    realSpeed = (state.slowed) ? realSpeed/2 : realSpeed;
    updateState();
    if (movState[0]){
        x -= realSpeed;
    }
    if (movState[1]){
        x += realSpeed;
    }
    x = (x < 1126) ? x : x - speed;
    x = (x > 60) ? x : x + speed;
}

void Player::shot(int lost, int type){
    Objects::shot(lost,type);
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
        state.buffed = true;
        state.timer.buffed = time(0) + 10;
        break;
    
    case 1:
        state.invulnerable = true;
        state.timer.invulnerable = time(0) + 5;
        lives++;
        lif_new_anim = 0;
        break;

    case 2:
        state.invulnerable = true;
        state.timer.invulnerable = time(0) + 10;
        state.buffed = true;
        state.timer.buffed = time(0) + 10;
        break;
    }
}

void Player::draw(){
    int design_lives = (lives < last_lives) ? lives : last_lives;
    al_draw_bitmap_region(spritesheet, 64*anim, 0, 64, 64, x, y, 0);
    for (int i = 1; i <= design_lives; i++){
        al_draw_bitmap_region(spritesheet_life, 64*lif_anim,0,64,64,0,950-(i*64),0);
    }
    for (int i = design_lives+1; i <= design_lives + std::abs(lives - last_lives); i++){
        al_draw_bitmap_region(spritesheet_life, 64*lif_new_anim,0,64,64,0,950-(i*64),0);
    }
};