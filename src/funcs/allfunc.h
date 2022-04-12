bool verifyCollision(int obj1[4], int obj2[4]);

bool verifyCollision(int obj1[4], int obj2[4]) //function to verify collisions between two entities
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