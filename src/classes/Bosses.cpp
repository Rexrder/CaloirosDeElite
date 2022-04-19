#include "Bosses.h"

Bosses::Bosses(int typ, double diff, bool m_b)
{
    difficulty = diff;
    type = typ;
    mega_boss = m_b;
    x = (mega_boss) ? WIDTH + 200 : -200;
    y = 300 - 64*type;

    size[0] = 128;
    size[1] = 126;

    ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_set_path_filename(path, "/res/font.ttf");
    font_boss = al_load_font(al_path_cstr(path, '/'), 20, 0);

    path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_set_path_filename(path, "/res/sound/effects/spatt.ogg");
    sounds.sp_att = al_load_sample(al_path_cstr(path, '/'));

    path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    switch (type)
    {
    case 0:
        size[0] = 121;
        size[1] = 121;
        color[0] = 100;
        color[1] = 155;
        color[2] = 255;
        shot_gap = 28;

        al_set_path_filename(path, "/res/fis.png");
        spritesheet = al_load_bitmap(al_path_cstr(path, '/'));
        break;

    case 1:
        size[0] = 65;
        size[1] = 117;
        color[0] = 251;
        color[1] = 242;
        color[2] = 54;
        shot_gap = 16;

        al_set_path_filename(path, "/res/med.png");
        spritesheet = al_load_bitmap(al_path_cstr(path, '/'));
        break;

    case 2:
        size[0] = 122;
        size[1] = 111;
        color[0] = 172;
        color[1] = 50;
        color[2] = 50;
        shot_gap = 54;

        al_set_path_filename(path, "/res/um.png");
        spritesheet = al_load_bitmap(al_path_cstr(path, '/'));
        break;
    }

    lives = (mega_boss) ? 150 * difficulty : 25 * difficulty;
    original_lives = lives;
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

int Bosses::getType()
{
    return type;
}

int Bosses::getShotGap(){
    return shot_gap;
}

bool Bosses::isMegaboss(){
    return mega_boss;
}

bool Bosses::shootSpecial()
{
    if (!state.buffed)
    {
        int probability = (difficulty <= 6) ? rand() % (int)floor(600 / difficulty) : rand() % (int)floor(600 / 6);
        if (probability == 0)
        {
            al_play_sample(sounds.sp_att, 0.7, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            state.buffed = true;
            state.timer.buffed = time(0) + 5;
        }
    }
    if (state.buffed && anim == 8)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Bosses::shot(int lost, int type)
{
    Objects::shot(lost, type);
    if (lives > 0)
    {
        state.invulnerable = true;
        state.timer.invulnerable = time(0) + 1;
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
        x += 5 + floor(speed * (1.00 - (lives / original_lives)));
    }
    else
    {
        x -= (5 + floor(speed * (1.00 - (lives / original_lives))));
    }
    if ((x > (WIDTH - 60 - size[0]) && moving_right) || (x < 60 && !moving_right))
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
    int vertical_origin = (type == 1) ? 0 : y + size[1] - 40;
    int life_bar = ceil((WIDTH - 10)*(lives / original_lives));
    life_bar = (life_bar < 0) ? 0 : life_bar;

    if (state.buffed && anim >= 5 && anim <= 8)
    {
        al_draw_filled_rectangle(x + shot_gap, vertical_origin, x + size[0] - shot_gap, 950, al_map_rgba(ceil(color[0] * ((anim - 4) / 4.00)), ceil(color[1] * ((anim - 4) / 4.00)), ceil(color[2] * ((anim - 4) / 4.00)), ceil(255 * ((anim - 4) / 4.00))));
    }
    al_draw_bitmap_region(spritesheet, size[0] * anim, 0, size[0], size[1], x, y, 0);

    if(mega_boss){
        std::string boss_str="BOSS LIFE";
        al_draw_text(font_boss, al_map_rgb(255, 255, 255), 10, 25, ALLEGRO_ALIGN_LEFT, boss_str.c_str());
        al_draw_filled_rectangle(10, 35, ceil((WIDTH - 10)*(lives / original_lives)),50, al_map_rgb(color[0],color[1],color[2]));
    }
};