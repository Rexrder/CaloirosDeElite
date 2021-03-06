#include "Bosses.h"
#include "Enemies.h"
#include "Bullets.h"
#include "Player.h"
#include "Bonus.h"
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
    std::list<Bonus *> bonusEntities;

    unsigned int score_buffs[2];
    bool enem_dir;
    double difficulty;
    std::string player_name;
    Player *player;
    unsigned int score;
    int mode;
    int slot;
    int wave;
    int level;
    int enemies_killed[4] = {0,0,0,0};
    int enemy_it;
    bool victory;
    int counter_wave;
    ALLEGRO_FONT *font;
    ALLEGRO_BITMAP *spr_skins;

public:
    Game();
    ~Game();
    void createEnemies(int, int);
    void animateEntities();
    bool moveEntities();
    void drawEntities();
    void playerMovement(ALLEGRO_EVENT event);
    void collisionHandler();
    void drawUI();
    void restart(int mod = 0, int pl_type = 0, int level = 0, int slot = 0, std::string player_n = "");
    void saveLoad();
};

#endif