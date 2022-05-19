#include "Game.h"
#include "Funcs.h"
#include "Player.h"
#include "Bosses.h"
#include "Enemies.h"
#include "Bonus.h"
#include <iostream>
#include <cmath>
#include <list>
#include <cstring>
#include <allegro5/allegro5.h>

Game::Game(int mod, int pl_type, int lev)
{
    int max_lifes;
    victory = false;
    ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_set_path_filename(path, "/res/font.ttf");
    font = al_load_font(al_path_cstr(path, '/'), 25, 0);
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
    player = new Player(pl_type, max_lifes);
    score_buffs[0] = std::ceil(difficulty * 2500);
    score_buffs[1] = std::ceil(difficulty * 10000);
}

void Game::restart(int mod, int pl_type, int lev)
{
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
    player = new Player(pl_type, max_lifes);
    score_buffs[0] = std::ceil(difficulty * 2500);
    score_buffs[1] = std::ceil(difficulty * 10000);
    bulletsEnemies.clear();
    bulletsAlly.clear();
    enemiesAvailable.clear();
    bossesAvailable.clear();
    bonusEntities.clear();
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

    counter_wave = (bossesAvailable.size() == 0 || (bossesAvailable.size() == 1 && mega_boss)) ? counter_wave + 1 : 0;

    if (mode == 0)
    {
        if(rand() % 20 == 0){
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
    else if (bossesAvailable.size() <= 2)
    {
        switch (level)
        {
        case 0:
            if (counter_wave >= 3)
            {
                bossesAvailable.push_back(new Bosses(enemy_it, difficulty + level));
                enemy_it++;
            }
            break;
        case 1:
            if (counter_wave >= 2)
            {
                bossesAvailable.push_back(new Bosses(enemy_it, difficulty + level));
                enemy_it++;
            }
            break;
        case 2:
            if (wave == 1)
            {
                bossesAvailable.push_back(new Bosses(2, difficulty, true));
            }
            if (counter_wave >= 2)
            {
                bossesAvailable.push_back(new Bosses(enemy_it, difficulty + level));
                enemy_it = (enemy_it == 0) ? 1 : 0;
            }
            break;
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
        for (Bonus *const &m : bonusEntities)
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

    std::list<Bullets*>::iterator n_bullets = bulletsEnemies.begin();
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

    std::list<Enemies*>::iterator n_enemies = enemiesAvailable.begin();
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

    std::list<Bonus*>::iterator n_bonus = bonusEntities.begin();
    while (n_bonus != bonusEntities.end())
    {
        (*n_bonus)->move();
        if ((*n_bonus)->getSize()[0] > WIDTH || (*n_bonus)->getState().erase)
        {
            if ((*n_bonus)->getState().erase)
            {
                score += std::ceil(2500 * difficulty);
            }
            delete (*n_bonus);
            n_bonus = bonusEntities.erase(n_bonus);
        }
        else
        {
            ++n_bonus;
        }
    }

    std::list<Bosses*>::iterator n_bosses = bossesAvailable.begin();
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
