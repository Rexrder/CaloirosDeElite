#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <iostream>
#include <string>
#include "Game.h"

#ifndef MENU_H
#define MENU_H

class Menu
{
public:
    Menu();
    ~Menu();
    bool setstate(ALLEGRO_EVENT, Game &, bool &);
    void draw();
    void animate();
    void restart();
    

private:
    int opt[2];
    ALLEGRO_FONT *font;
    ALLEGRO_FONT *font_title;
    int player_type;
    std::string options[23]= {"MAIN MENU","PLAY","HIGH SCORES","TROPHIES","SETTINGS","CREDITS","NEW GAME","LOAD GAME","ENDLESS","CUSTOMIZATION","SETTINGS","SLOT 1","SLOT 2","SLOT 3","EASY","NORMAL","HARD","HARDCORE","SLOT 1","SLOT 2", "SLOT 3","EEIC", "EGSI"};

};

#endif