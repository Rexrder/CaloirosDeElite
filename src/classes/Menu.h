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

    ALLEGRO_FONT *font;
    ALLEGRO_FONT *font_title;
    std::string player_name;
    std::list<std::string> highsc_list;
    ALLEGRO_BITMAP *spr_troph;
    ALLEGRO_BITMAP *spr_skin;
    ALLEGRO_BITMAP *spritesheet_life;

    int player_type;
    int slot;
    int slots[3][2] = {{0,0},{0,0},{0,0}};
    int anim = 0;
    int anim_mov = 1;
    int stats[7];

    int opt[2];
    int selects[13][2] = {{1,5},{6,8},{2,2},{3,3},{9,10},{5,5},{11,13},{18,20},{0,-1},{21,26},{0,-1},{14,17},{14,17},{14,17}};
    std::string options[27]= {"MAIN MENU","PLAY","HIGH SCORES","TROPHIES","SETTINGS","CREDITS","NEW GAME","LOAD GAME","ENDLESS","CUSTOMIZATION","TECHNICAL SETTINGS","SLOT 1","SLOT 2","SLOT 3","EASY","NORMAL","HARD","HARDCORE","SLOT 1","SLOT 2", "SLOT 3","EEIC", "EGSI","ECIV","EPOL","ETEXT","EMAT"};
    bool unlock_t[4] = {false, false, false, false};
    bool unlock_sk[6] = {true, false, false, false,false,false};
    std::string trophies[4] = {"FINAL DO SEMESTRE","CARROLACAR","MUDA DE CURSO","CALOIRO DE ELITE"};
};

#endif