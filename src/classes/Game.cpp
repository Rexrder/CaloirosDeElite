#include "Game.h"
#include "Funcs.h"
#include "Player.h"
#include "Bosses.h"
#include "Enemies.h"
#include <iostream>
#include <cmath>
#include <list>
#include <cstring>
#include <allegro5/allegro5.h>

Game::Game(int diff, int mod, int pl_type)
{
    ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_set_path_filename(path, "/res/font.ttf");
    font = al_load_font(al_path_cstr(path, '/'), 25, 0);
    difficulty = diff;
    enem_dir = false;
    mode = mod;
    score = 0;
    wave = 0;
    player = new Player(pl_type);
    score_buffs[0] = std::ceil(difficulty * 2500);
    score_buffs[1] = std::ceil(difficulty * 10000);
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

    if (/*wave % 5 == 0 &&*/ bossesAvailable.size() <= 2)
    {
        if (!mega_boss)
        {
            bossesAvailable.push_back(new Bosses(rand() % 3, difficulty, true));
        }
        bossesAvailable.push_back(new Bosses(rand() % 3, difficulty));
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
            enemiesAvailable.push_back(new Enemies(74 * i, -64 - 74 * j, enemy_app, difficulty));
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
            if (Funcs::verifyCollision(n->getSize(), m->getSize()) && m->getState().alive)
            {
                hit = true;
                m->shot(player->getDamage());
            };
        }
        for (Bosses *const &m : bossesAvailable)
        {
            if (Funcs::verifyCollision(n->getSize(), m->getSize()) && m->getState().alive)
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
        if (Funcs::verifyCollision(n->getSize(), player->getSize()) && player->getState().alive && !player->getState().invulnerable)
        {
            n->collide();
            player->shot();
            score = (score <= 50) ? 0 : score - 50;
            difficulty = (difficulty <= 2) ? 2 : difficulty - 0.4;
        };
    }

    for (Enemies *const &n : enemiesAvailable)
    {
        if (Funcs::verifyCollision(n->getSize(), player->getSize()) && player->getState().alive && !player->getState().invulnerable && n->getState().alive)
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

            if (Funcs::verifyCollision(area, player->getSize()) && player->getState().alive && !player->getState().invulnerable)
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
                    if (Funcs::verifyCollision(area, m->getSize()))
                    {
                        m->fortify();
                    }
                }
            }
        }
    }
}

int Game::moveEntities()
{
    int probability;

    auto n_bullets = bulletsEnemies.begin();
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

    if (enemiesAvailable.size() == 0)
    {
        difficulty += 0.2;
        createEnemies(10, 10);
    }

    auto n_enemies = enemiesAvailable.begin();
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

    auto n_bosses = bossesAvailable.begin();
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
            score = ((*n_bosses)->isMegaboss()) ? score + std::ceil(10000 * difficulty) : score + std::ceil(1000 * difficulty);
            player->buff((*n_bosses)->getType());
            delete (*n_bosses);
            n_bosses = bossesAvailable.erase(n_bosses);
        }
        else
        {
            ++n_bosses;
        }
    }

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

    if (player->getState().alive && !player->getState().stunned)
    {
        player->move();
    }

    return (player->getState().erase);
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
