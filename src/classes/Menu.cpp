#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <string>
#include "Menu.h"
#include "Game.h"

Menu::Menu()
{
    ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_set_path_filename(path, "/res/font.ttf");
    font = al_load_font(al_path_cstr(path, '/'), 50, 0);
    font_title = al_load_font(al_path_cstr(path, '/'), 80, 0);
    player_type = 0;
    opt[0] = 0;
    opt[1] = 1;
}

bool Menu::setstate(ALLEGRO_EVENT event, Game &game, bool &end)
{
    bool play = false;
    if (event.keyboard.keycode == ALLEGRO_KEY_W || event.keyboard.keycode == ALLEGRO_KEY_UP)
    {
        opt[1]--;
    }
    if (event.keyboard.keycode == ALLEGRO_KEY_S || event.keyboard.keycode == ALLEGRO_KEY_DOWN)
    {
        opt[1]++;
    }
    if (event.keyboard.keycode == ALLEGRO_KEY_ENTER)
    {
        opt[0] = opt[1];
        opt[1] = 100;
    }
    if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
    {
        if (opt[0] == 0)
        {
            end = true;
        }else if(opt[0] == 6)
        {
            opt[0] = 1;
        }
        else if(opt[0] == 11 || opt[0] == 12 || opt[0] == 13)
        {
            opt[0] = 6;
        } 
        else
        {
            restart();
        }
    }
    switch (opt[0])
    {

    case 0: // main
        if (opt[1] < 1)
        {
            opt[1] = 5; // May not be 5, deppends if there is or isnt trophies and some other options
        }
        if (opt[1] > 5)
        {
            opt[1] = 1;
        }
        break;
    case 1: // play
        if (opt[1] < 6)
        {
            opt[1] = 8;
        }
        if (opt[1] > 8)
        {
            opt[1] = 6;
        }
        break;
    case 2: // settings
        break;
    case 3: // trophies
        break;
    case 4: // high score
        break;
    case 5:
        break; // credits
    case 6:    // new game
        if (opt[1] < 11)
        {
            opt[1] = 13; // slo1, slot2, slot3
        }
        if (opt[1] > 13)
        {
            opt[1] = 11;
        }
        break;
    case 7: // load game
        if (opt[1] < 12)
        {
            opt[1] = 14; // easy, normal, hard ,hardcore
        }
        if (opt[1] > 14)
        {
            opt[1] = 12;
        }
    case 8: // endless
        game.restart(0, player_type, 0);
        play = true;
        break;
    case 9: // costumization

        if (opt[1] < 15)
        {
            opt[1] = 20;
        }
        if (opt[1] > 20)
        {
            opt[1] = 15;
        }

        break;
    case 10: // technical settings
        break;
    case 11:
        if (opt[1] < 14)
        {
            opt[1] = 17;
        }
        if (opt[1] > 17)
        {
            opt[1] = 14;
        }
        break;

    case 12:
        if (opt[1] < 14)
        {
            opt[1] = 17;
        }
        if (opt[1] > 17)
        {
            opt[1] = 14;
        }
        break;

    case 13:
        if (opt[1] < 14)
        {
            opt[1] = 17;
        }
        if (opt[1] > 17)
        {
            opt[1] = 14;
        }
        break;

    case 14:
        game.restart(1, player_type, 0);
        play = true;
        break;

    case 15:
        game.restart(2, player_type, 0);
        play = true;
        break;

    case 16:
        game.restart(3, player_type, 0);
        play = true;
        break;

    case 17:
        game.restart(4, player_type, 0);
        play = true;
        break;

    case 18:

        break;

    default:

        break;

        // meter cena das dificuldades
    };
    if (play)
    {
        restart();
    }
    return play;
}

void Menu::restart()
{
    opt[0] = 0;
    opt[1] = 1;
}

void Menu::draw()
{
    int rang[2];
    int b = 0;

    al_draw_text(font_title, al_map_rgb(255, 255, 255), WIDTH / 2, 100, ALLEGRO_ALIGN_CENTER, "CALOIROS DE ELITE");
    switch (opt[0])
    {
    case 0: // main
        rang[0] = 1;
        rang[1] = 5;
        break;
    case 1: // play
        rang[0] = 6;
        rang[1] = 8;
        break;
    case 6: // new game
        rang[0] = 11;
        rang[1] = 13;
        break;
    case 11: // slot 1
        rang[0] = 14;
        rang[1] = 17;
        break;

    case 12: // slot 2
        rang[0] = 14;
        rang[1] = 17;
        break;

    case 13: // slot 3
        rang[0] = 14;
        rang[1] = 17;
        break;

    default:
        rang[0] = 0;
        rang[1] = -1;
    };

    for (size_t i = rang[0]; i <= rang[1]; i++)
    {
        if (i == opt[1])
        {
            al_draw_text(font, al_map_rgb(155, 173, 183), WIDTH / 2, 300 + 80 * b, ALLEGRO_ALIGN_CENTER, options[i].c_str());
        }
        else
        {
            al_draw_text(font, al_map_rgb(255, 255, 255), WIDTH / 2, 300 + 80 * b, ALLEGRO_ALIGN_CENTER, options[i].c_str());
        }
        b++;
    }
}

void Menu::animate()
{
}

Menu::~Menu()
{
}