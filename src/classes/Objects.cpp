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
    posSize[0]=x;
    posSize[1]=y;
    posSize[2]=size[0];
    posSize[3]=size[1];

    return posSize;
}