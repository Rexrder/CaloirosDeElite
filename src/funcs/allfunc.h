bool verifyCollision(int obj1[4], int obj2[4]);

bool verifyCollision(int obj1[4], int obj2[4])
{
    bool overlap[2] = {false, false};
    int dis[2] ={obj1[0] - obj2[0], obj1[1] - obj2[1]};

    for (int i = 0; i < 2; i++)
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

    if (overlap[0] && overlap[1]){
        return true;
    }
    else
    {
        return false;
    }
};