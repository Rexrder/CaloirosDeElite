#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <string>
#include "Menu.h"
#include "Game.h"
#include "Funcs.h"

Menu::Menu()
{
    ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_set_path_filename(path, "/res/font.ttf");
    font = al_load_font(al_path_cstr(path, '/'), 50, 0);
    font_title = al_load_font(al_path_cstr(path, '/'), 80, 0);

    player_type = 0;
    player_name = NULL;
    
    slot = 0;
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
        switch (opt[0])
        {
        case 8:
            game.restart(0, player_type, 0);
            play = true;
            break;

        default:
            opt[0] = opt[1];
            opt[1] = 100;
            break;
        }
    }
    if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
    {
        opt[1] = 100;
        if (opt[0] == 0)
        {
            end = true;
        }
        else if (opt[0] == 6)
        {
            opt[0] = 1;
        }
        else if (opt[0] == 11 || opt[0] == 12 || opt[0] == 13)
        {
            opt[0] = 6;
        }
        else
        {
            restart();
        }
    }
    rangeDef(opt[1], selects[opt[0]]);

    switch (opt[0])
    {
    case 8: // endless

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
    int amount = (selects[opt[0]][1] - selects[opt[0]][0] > 0) ? (selects[opt[0]][1] - selects[opt[0]][0] - 1) : 0;
    int b = 0;

    al_draw_text(font_title, al_map_rgb(255, 255, 255), WIDTH / 2, 100, ALLEGRO_ALIGN_CENTER, "CALOIROS DE ELITE");

    switch (opt[0])
    {
    case 9: // main
        break;

    default:
        for (int i = selects[opt[0]][0]; i <= selects[opt[0]][1]; i++)
        {
            if (i == opt[1])
            {
                al_draw_text(font, al_map_rgb(155, 173, 183), WIDTH / 2, HEIGHT / 2 + 80 * b - 40 * amount, ALLEGRO_ALIGN_CENTER, options[i].c_str());
            }
            else
            {
                al_draw_text(font, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT / 2 + 80 * b - 40 * amount, ALLEGRO_ALIGN_CENTER, options[i].c_str());
            }
            b++;
        }
        break;
    };
}

void Menu::animate()
{
}

Menu::~Menu()
{
}