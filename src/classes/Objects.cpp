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

State Objects::getState(){
    return state;
}

void Objects::shot(int lost, int ch_state){
    lives -= lost;
    if (lives <= 0){
        state.alive = false;
    }
    switch (ch_state)
    {
    case 1:
        state.hacked = true;
        state.timer.hacked = time(0) + 6;
        break;
    case 2:
        state.slowed = true;
        state.timer.slowed = time(0) + 6;
        break;
    case 3:
        state.stunned = true;
        state.timer.stunned = time(0) + 3;
        break;
    }
}

void Objects::updateState(){
    if (state.timer.buffed <= time(NULL) && state.buffed){
        state.buffed = false;
    }
    if (state.timer.invulnerable <= time(NULL) && state.invulnerable){
        state.invulnerable = false;
    }
    if (state.timer.hacked <= time(NULL) && state.hacked){
        state.hacked = false;
    }
    if (state.timer.slowed <= time(NULL) && state.slowed){
        state.slowed = false;
    }
    if (state.timer.stunned <= time(NULL) && state.stunned){
        state.stunned = false;
    }
}