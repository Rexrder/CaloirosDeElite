#include "Bosses.h"

Bosses::Bosses(int typ, double diff)
{
    x = -200;
    y = 300;
    difficulty = diff;
    type = typ;

    size[0] = 128;
    size[1] = 126;
    ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    switch (type)
    {
    case 0:
        size[0] = 121;
        size[1] = 121;
        color[0] = 100;
        color[1] = 155;
        color[2] = 255;

        al_set_path_filename(path, "/res/fis.png");
        spritesheet = al_load_bitmap(al_path_cstr(path, '/'));

        path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
        al_set_path_filename(path, "/res/sound/effects/fis_spatt.ogg");
        sounds.sp_att = al_load_sample(al_path_cstr(path, '/'));
        break;

    default:
        break;
    }

    lives = 25 * difficulty;
    speed = (floor(5 * diff) >= 30) ? 30 : floor(5 * diff);
}

Bosses::~Bosses()
{
    al_destroy_bitmap(spritesheet);
    al_destroy_sample(sounds.sp_att);
}

Bullets *Bosses::shootNormal()
{
    return new Bullets(x + size[0] - 16, y + 64, false);
}

int Bosses::getType(){
    return type;
}

bool Bosses::shootSpecial()
{
    if (!state.buffed){
        int probability = (difficulty <= 6) ? rand() % (int)floor(600 / difficulty) : rand() % (int)floor(600 / 6);
        if (probability == 0)
        {
            al_play_sample(sounds.sp_att, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            state.buffed = true;
            state.timer.buffed = time(0) + 5;
        }
    }
    if(state.buffed && anim == 8){
        return true;
    }
    else{
        return false;
    }
}

void Bosses::shot(int lost, int type)
{
    Objects::shot(lost, type);
    if (lives > 0)
    {
        state.invulnerable = true;
        state.timer.invulnerable = time(0)+1;
    }
}

void Bosses::animate()
{
    if (!state.alive)
    {
        animation(12, 14, &anim, &anim_mov, false);
    }
    else if (state.buffed)
    {
        state.buffed = !animation(5, 8, &anim, &anim_mov, true);
    }
    else if (state.slowed)
    {
        animation(9, 11, &anim, &anim_mov, true);
    }
    else if (state.invulnerable)
    {
        state.invulnerable = !animation(3, 4, &anim, &anim_mov, true);
    }
    else
    {
        animation(0, 2, &anim, &anim_mov, true);
    }

    if (anim == 14)
    {
        state.erase = true;
    }
}

void Bosses::move()
{
    updateState();
    if (moving_right)
    {
        x += 5 + floor(speed * (1.00 - (lives / (25.00 * difficulty))));
    }
    else
    {
        x -= (5 + floor(speed * (1.00 - (lives / (25.00 * difficulty)))));
    }
    if ((x > (1170 - size[0]) && moving_right) || (x < 60 && !moving_right))
    {
        changeMove();
    }
}

void Bosses::changeMove()
{
    moving_right = !moving_right;
}

void Bosses::draw()
{
    if (state.buffed && anim >= 5 && anim <= 8)
    {
        al_draw_filled_rectangle(x + 28, y + size[1] - 40, x + size[0] - 28, 950, al_map_rgba(ceil(color[0] * ((anim - 4) / 4.00)), ceil(color[1] * ((anim - 4) / 4.00)), ceil(color[2] * ((anim - 4) / 4.00)), ceil(255 * ((anim - 4) / 4.00))));
    }
    al_draw_bitmap_region(spritesheet, size[0] * anim, 0, size[0], size[1], x, y, 0);
};