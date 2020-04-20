#include "fantastic-train.hpp"

namespace fantastictrain 
{
    using graphbases::COORDS_DATATYPE;
    using graphbases::coords;
    using namespace bases;

    void run(void)
    {
        grapher::initPoint(&graph[home]);
        pathfinder::calculateWay(target);
    }

    void init(void)
    {
        for(uint8_t i = 0; i < graphSize; i++)
        {
            for(uint8_t j = 0; j < sizeof(*graphbases::graphPoint::targets); j++)
            {
                graph[i].targets[j] = NULL;
            }
            graph[i].calculated = false;
            graph[i].i = i;
            graph[i].numOfTargets = 0;
            graph[i].weight = INF;
            ways[i] = target;   
        }

        for(uint8_t i = 0; i < numOfObstacles; i++)
        {
            obstacles[i].c = {0,0};
            obstacles[i].a = 0;
            obstacles[i].b = 0;
            obstacles[i].rot = 0;
            for(uint8_t j = 0; j < 4; j++)
            {
                obstacles[i].corners[j] = NULL;
            }
        }

        numOfObstacles = 0;
        graphSize = 2;

        graph[target].weight = 0;
        graph[target].i = static_cast<uint16_t>(target);

	    graph[home].i = home;
    }

    uint8_t* getWaysVector(void)
    {
        return ways;
    }

    void setHomeCoords(const coords c)
    {
        geometry::homeCoords = c;
    }
    
    void setTargetCoords(const coords c)
    {
        geometry::targetCoords = c;
    }

    void addObstacle(const coords c, const COORDS_DATATYPE a, const COORDS_DATATYPE b, const uint8_t rotation)
    {
        obstacles[numOfObstacles] = createObstacle(c.x, c.y, a, b, rotation);
        numOfObstacles++;
    }

} //namespace fantastictrain