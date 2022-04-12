#include "Enemies.cpp"
class Game
{
private:
    std::list<Bullets*> bulletsEnemies;
    std::list<Bullets*> bulletsAlly;
    std::list<Enemies*> enemiesAvailable;
    bool enem_dir;
    double difficulty;
    Player player = *new Player(518, 880);
    int score;
    int mode;
    ALLEGRO_FONT *font;
public:
    Game(int diff, int mod);
    ~Game();
    void createEnemies(int, int);
    void animateEntities();
    int moveEntities();
    void drawEntities();
    void playerMovement(ALLEGRO_EVENT event);
    void collisionHandler();
    void drawUI();
};