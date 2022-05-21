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
    int selects[23][2] = {{1,5},{6,8},{2,2},{0,-1},{9,10},{0,-1},{11,13},{18,20},{0,-1},{21,22},{0,-1},{14,17},{14,17},{14,17}};

    ALLEGRO_FONT *font;
    ALLEGRO_FONT *font_title;
    std::string player_name;
    std::list<std::string> highsc_list;

    int player_type;
    int slot;
    int slots[3][2];

    std::string options[23]= {"MAIN MENU","PLAY","HIGH SCORES","TROPHIES","SETTINGS","CREDITS","NEW GAME","LOAD GAME","ENDLESS","CUSTOMIZATION","TECHNICAL SETTINGS","SLOT 1","SLOT 2","SLOT 3","EASY","NORMAL","HARD","HARDCORE","SLOT 1","SLOT 2", "SLOT 3","EEIC", "EGSI"};

};

#endif