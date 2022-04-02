#include "Game.h"

Game::Game(int diff = 1, int mod = 0)
{
    difficulty = diff;
    enem_dir = false;
}

Game::~Game()
{
}

void Game::createEnemies(int x_n, int y_n)
{
    for (int i = 0; i < x_n; i++)
    {
        for (int j = 0; j < y_n; j++)
        {
            if (j < y_n/3)
            {
                enemiesAvailable.push_back(new Enemies( 74 * i, -64 - 74 * j, 2, difficulty));
            }
            else if (j < 4*y_n/5)
            {
                enemiesAvailable.push_back(new Enemies( 74 * i, -64 - 74 * j, 1, difficulty));
            }
            else
            {
                enemiesAvailable.push_back(new Enemies( 74 * i, -64 - 74 * j, 0, difficulty));
            }

            ;
        }
    }
}

bool Game::collisionHandler()
{
    bool hit = false;
    for (Bullets *n : bulletsAlly)
    {
        for (Enemies *m : enemiesAvailable)
        {
            if (verifyCollision((*n).getSize(), (*m).getSize()))
            {
                hit = true;
                (*m).~Enemies();
                enemiesAvailable.remove(m);
            };
        }
        if (hit)
        {
            hit = false;
            (*n).~Bullets();
            bulletsAlly.remove(n);
        }
    }

    for (Bullets *n : bulletsEnemies)
    {
        if (verifyCollision((*n).getSize(), player.getSize()))
        {
            (*n).~Bullets();
            bulletsEnemies.remove(n);
            return true;
        };
    }
    return false;
}

void Game::moveEntities()
{
    for (Bullets *n : bulletsEnemies)
    {
        (*n).move();
        if ((*n).isAlive())
        {
            (*n).~Bullets();
            bulletsEnemies.remove(n);
        }
    }

    for (Bullets *n : bulletsAlly)
    {
        (*n).move();
        if ((*n).isAlive())
        {
            (*n).~Bullets();
            bulletsAlly.remove(n);
        }
    }

    if (enemiesAvailable.size() == 0)
    {
        difficulty += 0.2;
        createEnemies(10, 10);
    }

    for (Enemies *n : enemiesAvailable)
    {
        if ((*n).move(enemiesAvailable.size()))
        {
            enem_dir = true;
        }
        if ((rand() % (int)floor(500/difficulty)) == 0)
        {
            bulletsEnemies.push_back((*n).shoot());
        };
        if ((*n).getSize()[1] > 1100){
            (*n).~Enemies();
            enemiesAvailable.remove(n);
        }
    }
    if (enem_dir)
    {
        for (Enemies *n : enemiesAvailable)
        {
            (*n).changeMove();
        }
        enem_dir = false;
    }
    player.move();
}

void Game::animateEntities()
{
    player.animate();
    for (Enemies *n : enemiesAvailable)
    {
        (*n).animate();
    }
}

void Game::drawEntities()
{
    player.draw();
    for (Bullets *n : bulletsEnemies)
    {
        (*n).draw();
    }

    for (Bullets *n : bulletsAlly)
    {
        (*n).draw();
    }

    for (Enemies *n : enemiesAvailable)
    {
        (*n).draw();
    }
}

void Game::playerMovement(ALLEGRO_EVENT event)
{
    switch (event.type)
    {
    case ALLEGRO_EVENT_KEY_DOWN:
        switch (event.keyboard.keycode)
        {
        case ALLEGRO_KEY_A:
            cout << "A";
            player.changeMove(true, 0);
            break;

        case ALLEGRO_KEY_D:
            cout << "B";
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
            cout << "A";
            player.changeMove(false, 0);
            break;

        case ALLEGRO_KEY_D:
            cout << "B";
            player.changeMove(false, 1);
            break;
        }
        break;
    }
}
