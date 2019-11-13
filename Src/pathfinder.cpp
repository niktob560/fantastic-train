#include "pathfinder.hpp"
#include "bases.hpp"
#include "math.hpp"

namespace pathfinder
{

    using namespace bases;

    bool hasUncalculatedPoints(const struct graphPoint *p)
    {
        for(size_t i = 0; i < (p->numOfTargets); i++)
        {
            if(!(p->targets[i]->calculated))
                return true;
        }
        return false;
    }

    struct graphPoint* getMinTarget(const struct graphPoint *p)
    {
        COORDS_DATATYPE w = INF;
        struct graphPoint * pp = 0;
        for(size_t i = 0; i < p->numOfTargets; i++)
        {
            if(!p->targets[i]->calculated && p->targets[i]->weight <= w)
            {
                w = p->targets[i]->weight;
                pp = p->targets[i];
            }
        }
        return pp;
    }

    void calculateWay(size_t index) 
    {
        if(graph[index] == NULL || graph[index]->calculated)
            return;
        graph[index]->calculated = true;
        
        if(graph[index]->numOfTargets == 0)
            return;

        struct graphPoint * watch;
        COORDS_DATATYPE w;

        for(size_t i = 0; i < graph[index]->numOfTargets; i++)
        {
            watch = graph[index]->targets[i];
            w = getWayPrice(graph[index], watch);
            if(watch->weight > graph[index]->weight + w)
            {
                watch->weight = w + graph[index]->weight;
                ways[watch->i] = index;
            }
        }

        while (hasUncalculatedPoints(graph[index]))
        {
            watch = getMinTarget(graph[index]);
            calculateWay(watch->i);
        }
    }


    COORDS_DATATYPE getWayPrice(struct graphPoint *start, struct graphPoint *end)
    {
        return sqrt(pow(start->c.x - end->c.x, 2) + pow(start->c.y - end->c.y, 2));
    }

    COORDS_DATATYPE getWayPrice(struct coords *start, struct coords *end)
    {
        return sqrt(pow(start->x - end->x, 2) + pow(start->y - end->y, 2));
    }

}