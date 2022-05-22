#include "Game.h"
#include "Funcs.h"
#include "Player.h"
#include "Bosses.h"
#include "Enemies.h"
#include "Bonus.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <list>
#include <string>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

Game::Game()
{
    ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_set_path_filename(path, "/res/font.ttf");
    font = al_load_font(al_path_cstr(path, '/'), 25, 0);
    path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_set_path_filename(path, "/res/skins.png");
    spr_skins = al_load_bitmap(al_path_cstr(path, '/'));
}

void Game::restart(int mod, int pl_type, int lev, int slot, std::string player_n)
{
    this->slot = slot;
    int max_lifes;
    victory = false;
    switch (mod)
    {
    case 0:
        difficulty = 3.4;
        max_lifes = 5;
        break;

    case 1:
        difficulty = 1.6;
        max_lifes = 5;
        break;

    case 2:
        difficulty = 2.8;
        max_lifes = 5;
        break;

    case 3:
        difficulty = 4;
        max_lifes = 5;
        break;

    case 4:
        difficulty = 4;
        max_lifes = 1;
        break;
    }
    enem_dir = false;

    mode = mod;
    level = lev;
    score = 0;
    wave = 0;
    counter_wave = 5;
    enemy_it = 0;
    player_name = player_n;
    for (int i = 0; i <= 3; i++)
    {
        enemies_killed[i] = 0;
    }

    player = new Player(pl_type, max_lifes,spr_skins);
    score_buffs[0] = std::ceil(difficulty * 2500);
    score_buffs[1] = std::ceil(difficulty * 10000);
    bulletsEnemies.clear();
    bulletsAlly.clear();
    enemiesAvailable.clear();
    bossesAvailable.clear();
    bonusEntities.clear();
    std::cout << level;
}

Game::~Game()
{
}

void Game::drawUI()
{
    if (mode == 0)
    {
        std::string score_str = "SCORE: " + std::to_string(score);
        al_draw_text(font, al_map_rgb(255, 255, 255), WIDTH, 25, ALLEGRO_ALIGN_RIGHT, score_str.c_str());
    }
};

void Game::createEnemies(int x_n, int y_n)
{
    int rand_val;
    int enemy_app;
    bool mega_boss = false;

    wave++;

    for (Bosses *const &n : bossesAvailable)
    {
        if (n->isMegaboss())
        {
            mega_boss = true;
        }
    }

    if (mode == 0)
    {
        if (rand() % 20 == 0)
        {
            bonusEntities.push_back(new Bonus(difficulty));
        }

        if (bossesAvailable.size() <= 2)
        {
            if (!mega_boss)
            {
                bossesAvailable.push_back(new Bosses(rand() % 3, difficulty, true));
            }
            bossesAvailable.push_back(new Bosses(rand() % 3, difficulty));
        }
    }
    else if (bossesAvailable.size() <= 2 && enemy_it < 3)
    {
        if (level == 2)
        {
            if (wave == 1)
            {
                bossesAvailable.push_back(new Bosses(2, difficulty, true));
            }
            if (wave % 2 == 0)
            {
                bossesAvailable.push_back(new Bosses(enemy_it, difficulty + level));
                enemy_it = (enemy_it == 0) ? 1 : 0;
            }
        }
        else
        {
            bossesAvailable.push_back(new Bosses(enemy_it, difficulty + level));
            enemy_it++;
        }
    }

    for (int i = 0; i < x_n; i++)
    {
        for (int j = 0; j < y_n; j++)
        {
            rand_val = rand();
            if (player->getType() == 0)
            {
                enemy_app = (rand_val % 5) + 1;
            }
            else
            {
                enemy_app = ((rand_val % 5) + 1 != player->getType()) ? (rand_val % 5) + 1 : 0;
            }
            enemiesAvailable.push_back(new Enemies(74 * i, -64 - 74 * j, enemy_app, difficulty,spr_skins));
        }
    }
}

void Game::collisionHandler()
{
    int area[4];
    bool hit = false;
    for (Bullets *const &n : bulletsAlly)
    {
        for (Enemies *const &m : enemiesAvailable)
        {
            if (verifyCollision(n->getSize(), m->getSize()) && m->getState().alive)
            {
                hit = true;
                m->shot(player->getDamage());
            };
        }
        for (Bosses *const &m : bossesAvailable)
        {
            if (verifyCollision(n->getSize(), m->getSize()) && m->getState().alive)
            {
                hit = true;
                m->shot(player->getDamage());
            };
        }
        for (Bonus *const &m : bonusEntities)
        {
            if (verifyCollision(n->getSize(), m->getSize()) && m->getState().alive)
            {
                hit = true;
                m->shot(player->getDamage());
            };
        }
        if (hit)
        {
            hit = false;
            n->collide();
        }
    }

    for (Bullets *const &n : bulletsEnemies)
    {
        if (verifyCollision(n->getSize(), player->getSize()) && player->getState().alive && !player->getState().invulnerable)
        {
            n->collide();
            player->shot();
            score = (score <= 50) ? 0 : score - 50;
            difficulty = (difficulty <= 2) ? 2 : difficulty - 0.4;
        };
    }

    for (Enemies *const &n : enemiesAvailable)
    {
        if (verifyCollision(n->getSize(), player->getSize()) && player->getState().alive && !player->getState().invulnerable && n->getState().alive)
        {
            player->shot(10);
            score = (score <= 100) ? 0 : score - 100;
        };
    }

    for (Bosses *const &n : bossesAvailable)
    {
        if (n->shootSpecial())
        {
            area[0] = n->getSize()[0] + n->getShotGap();
            area[1] = 0;
            area[2] = n->getSize()[2] - n->getShotGap();
            area[3] = HEIGHT;

            if (verifyCollision(area, player->getSize()) && player->getState().alive && !player->getState().invulnerable)
            {
                switch (n->getType())
                {
                case 0:
                    player->shot(2, 1);
                    break;

                case 1:
                    player->shot(1, 2);
                    break;

                case 2:
                    player->shot(10, 3);
                    break;
                }
                score = (score <= 100) ? 0 : score - 100;
            };

            if (n->getType() == 1)
            {
                for (Enemies *const &m : enemiesAvailable)
                {
                    if (verifyCollision(area, m->getSize()))
                    {
                        m->fortify();
                    }
                }
            }
        }
    }
}

bool Game::moveEntities()
{
    int probability;
    bool end = false;

    std::list<Bullets *>::iterator n_bullets = bulletsEnemies.begin();
    while (n_bullets != bulletsEnemies.end())
    {
        (*n_bullets)->move();
        if ((*n_bullets)->getState().erase)
        {
            delete (*n_bullets);
            n_bullets = bulletsEnemies.erase(n_bullets);
        }
        else
        {
            ++n_bullets;
        }
    }

    n_bullets = bulletsAlly.begin();
    while (n_bullets != bulletsAlly.end())
    {
        (*n_bullets)->move();
        if ((*n_bullets)->getState().erase)
        {
            delete (*n_bullets);
            n_bullets = bulletsAlly.erase(n_bullets);
            --n_bullets;
        }
        else
        {
            ++n_bullets;
        }
    }

    if (enemiesAvailable.size() == 0 && !victory)
    {
        difficulty += 0.2;
        createEnemies(10, 10);
    }

    if (enemiesAvailable.size() == 0 && bossesAvailable.size() == 0 && bulletsEnemies.size() == 0 && victory)
    {
        player->win();
    }

    std::list<Enemies *>::iterator n_enemies = enemiesAvailable.begin();
    while (n_enemies != enemiesAvailable.end())
    {
        if ((*n_enemies)->move(enemiesAvailable.size()))
        {
            enem_dir = true;
        }
        if (bossesAvailable.size() == 0)
        {
            probability = (difficulty <= 6) ? rand() % (int)std::floor(600 / difficulty) : rand() % (int)std::floor(600 / 6);
        }
        else
        {
            probability = (difficulty <= 6) ? rand() % (int)std::floor(1200 / difficulty) : rand() % (int)std::floor(1200 / 6);
        }
        if (probability == 0 && (*n_enemies)->getSize()[1] > 0)
        {
            bulletsEnemies.push_back((*n_enemies)->shoot());
        };
        if ((*n_enemies)->getSize()[1] > 1100 || (*n_enemies)->getState().erase)
        {
            if ((*n_enemies)->getState().erase)
            {
                score += std::ceil(10 * difficulty);
                enemies_killed[0]++;
            }
            delete (*n_enemies);
            n_enemies = enemiesAvailable.erase(n_enemies);
        }
        else
        {
            ++n_enemies;
        }
    }
    if (enem_dir)
    {
        for (Enemies *const &n : enemiesAvailable)
        {
            n->changeMove();
        }
        enem_dir = false;
    }

    std::list<Bonus *>::iterator n_bonus = bonusEntities.begin();
    while (n_bonus != bonusEntities.end())
    {
        (*n_bonus)->move();
        if ((*n_bonus)->getSize()[0] > WIDTH || (*n_bonus)->getState().erase)
        {
            if ((*n_bonus)->getState().erase)
            {
                score += std::ceil(2500 * difficulty);
                enemies_killed[1]++;
            }
            delete (*n_bonus);
            n_bonus = bonusEntities.erase(n_bonus);
        }
        else
        {
            ++n_bonus;
        }
    }

    std::list<Bosses *>::iterator n_bosses = bossesAvailable.begin();
    while (n_bosses != bossesAvailable.end())
    {
        (*n_bosses)->move();
        probability = (difficulty <= 6) ? rand() % (int)std::floor(300 / difficulty) : rand() % (int)std::floor(300 / 6);
        if (probability == 0 && (*n_bosses)->getSize()[0] > 0)
        {
            bulletsEnemies.push_back((*n_bosses)->shootNormal());
        };
        if ((*n_bosses)->getState().erase)
        {
            if (!(*n_bosses)->isMegaboss())
            {
                score += std::ceil(1000 * difficulty);
                enemies_killed[2]++;
            }
            else
            {
                score += std::ceil(10000 * difficulty);
                enemies_killed[3]++;
            }
            if (mode != 0)
            {
                if ((*n_bosses)->getType() == 2)
                {
                    victory = true;
                }
            }
            player->buff((*n_bosses)->getType());
            delete (*n_bosses);
            n_bosses = bossesAvailable.erase(n_bosses);
        }
        else
        {
            ++n_bosses;
        }
    }

    if (mode == 0)
    {
        if (score > score_buffs[0])
        {
            player->buff(1);
            score_buffs[0] += std::ceil(2500 * difficulty);
            if (score > score_buffs[1])
            {
                player->buff(2);
                score_buffs[1] += std::ceil(10000 * difficulty);
            }
        }
    }
    else
    {
        if (player->getState().erase && victory && level != 2)
        {
            restart(mode, player->getType(), level + 1);
        }
    }

    if (player->getState().alive && !player->getState().stunned)
    {
        player->move();
    }
    if (player->getState().erase && (!victory || level == 2))
    {
        end = true;
    }

    return end;
}

void Game::animateEntities()
{
    player->animate();
    for (Enemies *const &n : enemiesAvailable)
    {
        n->animate();
    }
    for (Bosses *const &n : bossesAvailable)
    {
        n->animate();
    }
    for (Bonus *const &n : bonusEntities)
    {
        n->animate();
    }
}

void Game::drawEntities()
{
    player->draw();

    for (Bullets *const &n : bulletsEnemies)
    {
        n->draw();
    }

    for (Enemies *const &n : enemiesAvailable)
    {
        n->draw();
    }

    for (Bosses *const &n : bossesAvailable)
    {
        n->draw();
    }

    for (Bonus *const &n : bonusEntities)
    {
        n->draw();
    }

    for (Bullets *const &n : bulletsAlly)
    {
        n->draw();
    }
    drawUI();
}

void Game::playerMovement(ALLEGRO_EVENT event)
{
    if (player->getState().alive)
    {
        switch (event.type)
        {
        case ALLEGRO_EVENT_KEY_DOWN:
            if (event.keyboard.keycode == ALLEGRO_KEY_A || event.keyboard.keycode == ALLEGRO_KEY_LEFT)
            {
                player->changeMove(true, 0);
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_D || event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
            {
                player->changeMove(true, 1);
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_L || event.keyboard.keycode == ALLEGRO_KEY_SPACE)
            {
                if (!player->getState().hacked)
                {
                    bulletsAlly.push_back(player->shoot());
                }
            }
            break;

        case ALLEGRO_EVENT_KEY_UP:
            if (event.keyboard.keycode == ALLEGRO_KEY_A || event.keyboard.keycode == ALLEGRO_KEY_LEFT)
            {
                player->changeMove(false, 0);
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_D || event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
            {
                player->changeMove(false, 1);
            }
            break;
        }
    }
}

void Game::saveLoad()
{
    int stats[7];
    int i = 0;
    std::string line;
    std::ifstream stats_i((".\\stats.txt"));
    if (stats_i.is_open())
    {
        while (std::getline(stats_i, line))
        {
            switch (i)
            {
            case 4:
                if(!victory){
                    stats[i] = (line.empty()) ? 1 : (1 + std::stoi(line));
                }
                break;
            case 5:
                if(victory){
                    stats[i] = (line.empty()) ? 1 : (1 + std::stoi(line));
                }
                break;
            case 6:
                if(victory && mode == 4){
                    stats[i] = (line.empty()) ? 1 : (1 + std::stoi(line));
                }
                break;

            default:
                stats[i] = (line.empty()) ? enemies_killed[i] : (enemies_killed[i] + std::stoi(line));
                break;
            }
            i++;
        }
    }

    std::ofstream stats_o((".\\stats.txt"));
    if (stats_o.is_open())
    {
        for (auto &n : stats)
        {
            stats_o << std::to_string(n).c_str() << "\n";  
        }
        stats_o.close();
    }

    if (mode == 0)
    {
        std::list<HighscrStat> highsc_list;
        int space_pos;
        HighscrStat play_hscr;

        std::ifstream highscores_i((".\\highscores.txt"));

        if (highscores_i.is_open())
        {
            play_hscr.player = player_name;
            play_hscr.score = score;
            highsc_list.push_back(play_hscr);

            while (std::getline(highscores_i, line))
            {
                space_pos = line.find(" ");
                play_hscr.player = line.substr(0, space_pos);
                play_hscr.score = std::stoi(line.substr(space_pos, -1));
                highsc_list.push_back(play_hscr);
            }

            highsc_list.sort(compareHighs);

            while (highsc_list.size() > 8)
            {
                highsc_list.pop_back();
            }
            highscores_i.close();
        }

        std::ofstream highscores_o((".\\highscores.txt"));
        if (highscores_o.is_open())
        {
            for (HighscrStat const &n : highsc_list)
            {
                highscores_o << n.player.c_str() << " " << std::to_string(n.score).c_str() << "\n";
            }
            highscores_o.close();
        }
    }
    else
    {
        std::string slots[3];
        i = 0;
        std::ifstream save_i((".\\save.txt"));
        if (save_i.is_open())
        {
            while (std::getline(save_i, line))
            {
                slots[i] = line;
                i++;
            }
            save_i.close();
        }

        std::ofstream save_o((".\\save.txt"));
        if (save_o.is_open())
        {
            if (mode == 4)
            {
                slots[slot] = "";
            }
            else
            {
                slots[slot] = (victory) ? std::to_string(mode) + " " + std::to_string(level + 1) : std::to_string(mode) + " " + std::to_string(level);
            }
            for (auto &n : slots)
            {
                save_o << n.c_str() << "\n";
            }
            save_o.close();
        }
    }
}
