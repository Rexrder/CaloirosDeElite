#include "Objects.h"

Objects::Objects(int xstart, int ystart)
{
    x = xstart;
    y = ystart;
}

Objects::~Objects()
{
}

int * Objects::getSize(){
    posSize[0] = x;
    posSize[1] = y;
    posSize[2] = size[0];
    posSize[3] = size[1];
    return posSize;
}

bool* Objects::isAlive(){
    return aliveErase;
}

void Objects::shot(int lost = 1){
    lives -= lost;
    if (lives <= 0){
        aliveErase[0]=false;
        anim = 5;
        anim_mov = 1;
    }
    else{
        anim = 4;
        anim_mov = -1;
    }
}