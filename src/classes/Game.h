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
public:
    Game();
    ~Game();
    void createEnemies(int, int);
    void animateEntities();
    void moveEntities();
    void drawEntities();
    void playerMovement(ALLEGRO_EVENT event);
    bool collisionHandler();
};