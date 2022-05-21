#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <string>
#include <iostream>
#include <fstream>
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
    player_name.clear();

    slot = 0;
    restart();
}

bool Menu::setstate(ALLEGRO_EVENT event, Game &game, bool &end)
{
    bool play = false;

    if (event.type == ALLEGRO_EVENT_KEY_DOWN)
    {
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
            if (opt[1] >= 18 && opt[1] <= 20)
            {
                if (slots[opt[1] - 18][0] != -1)
                {
                    game.restart(slots[opt[1] - 18][0], player_type, slots[opt[1] - 18][1], opt[1] - 18);
                    play = true;
                }
            }
            else
            {
                switch (opt[0])
                {
                case 8:
                    if (!player_name.empty())
                    {
                        game.restart(0, player_type, 0, slot, player_name);
                        play = true;
                    }
                    break;

                default:
                    opt[0] = opt[1];
                    opt[1] = 100;
                    break;
                }
            }
        }
        if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
        {
            player_name.clear();
            opt[1] = 100;
            if (opt[0] == 0)
            {
                end = true;
            }
            else if (opt[0] >= 6 && opt[0] <= 8)
            {
                opt[0] = 1;
            }
            else if (opt[0] >= 11 && opt[0] <= 13)
            {
                opt[0] = 6;
            }
            else
            {
                restart();
            }
        }
        if (event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && opt[0] == 8 && !player_name.empty())
        {
            player_name.pop_back();
        }
    }

    rangeDef(opt[1], selects[opt[0]]);

    switch (opt[0])
    {
    case 11:
        slot = 0;
        break;

    case 12:
        slot = 1;
        break;

    case 13:
        slot = 2;
        break;

    case 8: // endless
        if (event.type == ALLEGRO_EVENT_KEY_CHAR && event.keyboard.keycode != ALLEGRO_KEY_BACKSPACE && event.keyboard.keycode != ALLEGRO_KEY_ENTER && player_name.size() < 20)
        {
            std::cout << event.keyboard.unichar;
            player_name += ((char)event.keyboard.unichar);
        }
        break;
    case 14:
        game.restart(1, player_type, 0, slot);
        play = true;
        break;
    case 15:
        game.restart(2, player_type, 0, slot);
        play = true;
        break;
    case 16:
        game.restart(3, player_type, 0, slot);
        play = true;
        break;
    case 17:
        game.restart(4, player_type, 0, slot);
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
    player_name.clear();
    highsc_list.clear();

    std::ifstream highscores_i((".\\highscores.txt"));
    std::string line;

    if (highscores_i.is_open())
    {
        while (std::getline(highscores_i, line))
        {
            highsc_list.push_back(line);
        }
        highscores_i.close();
    }

    int i = 0;
    std::ifstream save_i((".\\save.txt"));
    if (save_i.is_open())
    {
        while (std::getline(save_i, line))
        {
            std::cout << line.front();
            slots[i][0] = (line.front() == (char)0) ? -1 : (int)(line.front() - 48);
            slots[i][1] = (line.front() == (char)0) ? -1 : (int)(line.back() - 48);
            i++;
        }
        save_i.close();
    }
}

void Menu::draw()
{
    int amount = (selects[opt[0]][1] - selects[opt[0]][0] > 0) ? (selects[opt[0]][1] - selects[opt[0]][0] - 1) : 0;
    int b = 0;

    al_draw_text(font_title, al_map_rgb(255, 255, 255), WIDTH / 2, 100, ALLEGRO_ALIGN_CENTER, "CALOIROS DE ELITE");

    switch (opt[0])
    {
    case 8: // endless
        al_draw_text(font, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT / 2 - 40, ALLEGRO_ALIGN_CENTER, "NAME");
        al_draw_text(font, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT / 2 + 40, ALLEGRO_ALIGN_CENTER, player_name.c_str());
        break;

    case 2:

        for (auto &n : highsc_list)
        {
            al_draw_text(font, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT / 2 + (80 * b) - (40 * (highsc_list.size() - 1)) + 40, ALLEGRO_ALIGN_CENTER, n.c_str());
            b++;
        }
        break;
    default:
        for (int i = selects[opt[0]][0]; i <= selects[opt[0]][1]; i++)
        {
            if (i == opt[1])
            {
                al_draw_text(font, al_map_rgb(155, 173, 183), WIDTH / 2, HEIGHT / 2 + (80 * b) - (40 * amount), ALLEGRO_ALIGN_CENTER, options[i].c_str());
            }
            else
            {
                al_draw_text(font, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT / 2 + (80 * b) - (40 * amount), ALLEGRO_ALIGN_CENTER, options[i].c_str());
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