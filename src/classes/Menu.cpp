#include "Menu.h"
#include <allegro5/allegro5.h>

void Menu::setstate(ALLEGRO_EVENT event ){


if(event.keyboard.keycode == ALLEGRO_KEY_W || event.keyboard.keycode== ALLEGRO_KEY_UP){

opt[1]--;
}
if (event.keyboard.keycode == ALLEGRO_KEY_S || event.keyboard.keycode == ALLEGRO_KEY_DOWN){
opt[1]++;
}
if (event.keyboard.keycode == ALLEGRO_KEY_ENTER){
    opt[0]=opt[1];
    opt[1]=100;

}

switch (opt[0]){

case 0:                 //main
if (opt[1]<1){
    opt[1]=5;       //May not be 5, deppends if there is or isnt trophies and some other options
}
if (opt[1]>5)
{
  opt[1]=1;
}
break;
case 1:             //play
if (opt[1]<6){
opt[1]=8;
}
if (opt[1]>8){
    opt[1]=6;
}
break;
case 2:         //settings
if (opt[1]<8){
opt[1]=9;
}
if (opt[1]>9){
    opt[1]=8;
}
break;
case 3:     //trophies
break;
case 4:         //high score
break;
case 5:
break;              //credits
case 6:             //new game 
if (opt[1]<10){
    opt[1]=12;       // easy, normal, hard ,hardcore
}    
if (opt[1]>12)
{
    opt[1]=10;
}
break;
case 7:             //load game
if (opt[1]<12){
    opt[1]=14;       // easy, normal, hard ,hardcore
}    
if (opt[1]>14)
{
    opt[1]=12;
}
case 8:         //endless
break;
case 9:         //costumization


if (opt[1]<15){
    opt[1]=20;       
}    
if (opt[1]>20)
{
    opt[1]=15;
}


break;
case 10:            //technical settings
break;          
case 11:            //load game 
// ns quantos loads fzr 
break;


//meter cena das dificuldades
















};




