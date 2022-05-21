#include "Funcs.h"

bool verifyCollision(int obj1[4], int obj2[4]) //function to verify collisions between two 
{
    bool overlap[2] = {false, false}; //vector which saves information of entities overlapping
    int dis[2] ={obj1[0] - obj2[0], obj1[1] - obj2[1]}; //vector to save distance between origin of entities

    for (int i = 0; i < 2; i++) //analyse relationship between distance and height/width
    {
        if (dis[i] < 0) 
        {
            if ((-dis[i]) <= obj1[2+i])
            {
                overlap[i] = true;
            }
        }
        else if (dis[i] <= obj2[2+i])
        {
            overlap[i] = true;
        }
    }
        return (overlap[0] && overlap[1]);
};

bool animation(int start, int end, int &anim, int &anim_mov, bool ping_pong){
    bool end_cycle = false;

    if (anim < start || anim > end){
        anim = start;
        anim_mov = 1;
    }else{
        if (anim == start){
            if (anim_mov == -1){
                end_cycle = true;
            }
            anim_mov = 1;
        }
        if (anim == end){
            if (ping_pong){
                anim_mov = -1;
            }
            else{
                end_cycle = true;
                anim = start - 1;
            }
        }
        anim += anim_mov;
    }
    return end_cycle;    
};

void rangeDef(int &val, int* rang){
    if (val < rang[0])
    {
        val = rang[1];
    }
    if (val > rang[1])
    {
        val = rang[0];
    }    
};

bool compareHighs(const HighscrStat& first, const HighscrStat& second){
    return (first.score > second.score);
};