#include "Bosses.h"
#include "Enemies.h"
#include "Bullets.h"
#include "Player.h"
#include <list>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro5.h>

#ifndef GAME_H
#define GAME_H
class Game
{
private:
    std::list<Bullets *> bulletsEnemies;
    std::list<Bullets *> bulletsAlly;
    std::list<Enemies *> enemiesAvailable;
    std::list<Bosses *> bossesAvailable;

    unsigned int score_buffs[2];
    bool enem_dir;
    double difficulty;
    Player *player;
    unsigned int score;
    int mode;
    int wave;
    ALLEGRO_FONT *font;

public:
    Game(int = 1, int = 0, int = 0);
    ~Game();
    void createEnemies(int, int);
    void animateEntities();
    int moveEntities();
    void drawEntities();
    void playerMovement(ALLEGRO_EVENT event);
    void collisionHandler();
    void drawUI();
};

#endif