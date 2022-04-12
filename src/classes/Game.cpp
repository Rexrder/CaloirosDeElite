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
}

Game::~Game()
{
}

void Game::drawUI()
{
    if (mode == 0)
    {
        std::string score_str = std::to_string(score);
        al_draw_text(font, al_map_rgb(255, 255, 255), 1200, 25, ALLEGRO_ALIGN_RIGHT, score_str.c_str());
    }
};

void Game::createEnemies(int x_n, int y_n)
{
    for (int i = 0; i < x_n; i++)
    {
        for (int j = 0; j < y_n; j++)
        {
            if (j < y_n / 3)
            {
                enemiesAvailable.push_back(new Enemies(74 * i, -64 - 74 * j, 2, difficulty));
            }
            else if (j < 4 * y_n / 5)
            {
                enemiesAvailable.push_back(new Enemies(74 * i, -64 - 74 * j, 1, difficulty));
            }
            else
            {
                enemiesAvailable.push_back(new Enemies(74 * i, -64 - 74 * j, 0, difficulty));
            }

            ;
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
            if (verifyCollision(n->getSize(), m->getSize()) && m->isAlive()[0])
            {
                score += ceil(10 * difficulty);
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
        if (verifyCollision(n->getSize(), player.getSize())&& player.isAlive()[0])
        {
            bulletsEnemies.remove(n);
            delete n;
            player.shot();
            score = (score <= 50) ?  0 : score - 50;
            difficulty = (difficulty <= 2) ?  2 : difficulty - 0.4;
        };
    }
}

int Game::moveEntities()
{
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
        if ((rand() % (int)floor(500 / difficulty)) == 0 && n->getSize()[1] > 0)
        {
            bulletsEnemies.push_back(n->shoot());
        };
        if (n->getSize()[1] > 1100 || n->isAlive()[1])
        {
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
    if (player.isAlive()[0]){
        player.move();
    }
    
    return (player.isAlive()[1]);
}

void Game::animateEntities()
{
    player.animate();
    for (Enemies *const &n : enemiesAvailable)
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

    for (Bullets *const &n : bulletsAlly)
    {
        n->draw();
    }
    drawUI();
}

void Game::playerMovement(ALLEGRO_EVENT event)
{
    if (player.isAlive()[0])
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
                bulletsAlly.push_back(player.shoot());
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
