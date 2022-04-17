#include "Game.h"

Game::Game(int diff = 1, int mod = 0)
{
    ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_set_path_filename(path, "/res/font.ttf");
    font = al_load_font(al_path_cstr(path, '/'), 25, 0);
    difficulty = diff;
    enem_dir = false;
    mode = mod;
    score = 0;
    wave = 0;

    score_buffs[0] = ceil(difficulty*2500);
    score_buffs[1] = ceil(difficulty*10000);
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

    wave++;

    if (/*wave % 5 == 0 &&*/ bossesAvailable.size() == 0)
    {
        bossesAvailable.push_back(new Bosses(2, difficulty, true));
        bossesAvailable.push_back(new Bosses(0, difficulty));
    }

    for (int i = 0; i < x_n; i++)
    {
        for (int j = 0; j < y_n; j++)
        {
            rand_val = rand();
            enemy_app = (rand_val % 5 != 0) ? rand_val % 5 : (rand_val % 5) + 1;
            enemiesAvailable.push_back(new Enemies(74 * i, -64 - 74 * j, enemy_app, difficulty));
        }
    }
}

void Game::collisionHandler()
{
    bool hit = false;
    for (Bullets *const &n : bulletsAlly)
    {
        for (Enemies *const &m : enemiesAvailable)
        {
            if (verifyCollision(n->getSize(), m->getSize()) && m->getState().alive)
            {
                hit = true;
                m->shot();
            };
        }
        for (Bosses *const &m : bossesAvailable)
        {
            if (verifyCollision(n->getSize(), m->getSize()) && m->getState().alive)
            {
                hit = true;
                m->shot();
            };
        }
        if (hit)
        {
            hit = false;
            bulletsAlly.remove(n);
            delete n;
        }
    }

    for (Bullets *const &n : bulletsEnemies)
    {
        if (verifyCollision(n->getSize(), player.getSize()) && player.getState().alive && !player.getState().invulnerable)
        {
            bulletsEnemies.remove(n);
            delete n;
            player.shot();
            score = (score <= 50) ? 0 : score - 50;
            difficulty = (difficulty <= 2) ? 2 : difficulty - 0.4;
        };
    }

    for (Enemies *const &n : enemiesAvailable)
    {
        if (verifyCollision(n->getSize(), player.getSize()) && player.getState().alive && !player.getState().invulnerable)
        {
            player.shot(10);
            score = (score <= 100) ? 0 : score - 100;
        };
    }

    for (Bosses *const &n : bossesAvailable)
    {
        if (n->shootSpecial())
        {
            int area[4] = {n->getSize()[0] + n->getShotGap(), n->getSize()[1], n->getSize()[2]- n->getShotGap(), HEIGHT};
            if (verifyCollision(area, player.getSize()) && player.getState().alive && !player.getState().invulnerable)
            {
                switch (n->getType())
                {
                case 0:
                    player.shot(2,1);
                    break;
                
                case 1:
                    player.shot(1,2);
                    break;
                
                case 2:
                    player.shot(10,3);
                    break;
                }
                score = (score <= 100) ? 0 : score - 100;
            };
        }
    }
}

int Game::moveEntities()
{
    int probability;

    for (Bullets *const &n : bulletsEnemies)
    {
        n->move();
        if (n->isAlive())
        {
            bulletsEnemies.remove(n);
            delete n;
        }
    }

    for (Bullets *const &n : bulletsAlly)
    {
        n->move();
        if (n->isAlive())
        {
            bulletsAlly.remove(n);
            delete n;
        }
    }

    if (enemiesAvailable.size() == 0)
    {
        difficulty += 0.2;
        createEnemies(10, 10);
    }

    for (Enemies *const &n : enemiesAvailable)
    {
        if (n->move(enemiesAvailable.size()))
        {
            enem_dir = true;
        }
        if (bossesAvailable.size() == 0)
        {
            probability = (difficulty <= 6) ? rand() % (int)floor(600 / difficulty) : rand() % (int)floor(600 / 6);
        }
        else
        {
            probability = (difficulty <= 6) ? rand() % (int)floor(1200 / difficulty) : rand() % (int)floor(1200 / 6);
        }
        if (probability == 0 && n->getSize()[1] > 0)
        {
            bulletsEnemies.push_back(n->shoot());
        };
        if (n->getSize()[1] > 1100 || n->getState().erase)
        {
            if (n->getState().erase)
            {
                score += ceil(10 * difficulty);
            }
            enemiesAvailable.remove(n);
            delete n;
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

    for (Bosses *const &n : bossesAvailable)
    {
        n->move();
        probability = (difficulty <= 6) ? rand() % (int)floor(300 / difficulty) : rand() % (int)floor(300 / 6);
        if (probability == 0 && n->getSize()[0] > 0)
        {
            bulletsEnemies.push_back(n->shootNormal());
        };
        if (n->getState().erase)
        {
            score = (n->isMegaboss()) ? score + ceil(10000 * difficulty) : score + ceil(1000 * difficulty);
            player.buff(n->getType());
            bossesAvailable.remove(n);
            delete n;
        }
    }

    if (score > score_buffs[0]){
        player.buff(1);
        score_buffs[0]+= ceil(2500*difficulty);
        if (score > score_buffs[1])
        {
            player.buff(2);
            score_buffs[1]+= ceil(10000*difficulty);
        }     
    }

    if (player.getState().alive && !player.getState().stunned)
    {
        player.move();
    }

    return (player.getState().erase);
}

void Game::animateEntities()
{
    player.animate();
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
    player.draw();

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
    if (player.getState().alive)
    {
        switch (event.type)
        {
        case ALLEGRO_EVENT_KEY_DOWN:
            switch (event.keyboard.keycode)
            {
            case ALLEGRO_KEY_A:
                player.changeMove(true, 0);
                break;

            case ALLEGRO_KEY_D:
                player.changeMove(true, 1);
                break;

            case ALLEGRO_KEY_L:
                if (!player.getState().hacked)
                {
                    bulletsAlly.push_back(player.shoot());
                }
                break;
            }
            break;

        case ALLEGRO_EVENT_KEY_UP:
            switch (event.keyboard.keycode)
            {
            case ALLEGRO_KEY_A:
                player.changeMove(false, 0);
                break;

            case ALLEGRO_KEY_D:
                player.changeMove(false, 1);
                break;
            }
            break;
        }
    }
}
