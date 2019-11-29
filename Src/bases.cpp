#include "bases.hpp"
#include "geometry.hpp"

struct graphbases::graphPoint **graph;
struct bases::obstacle *obstacles;

struct graphbases::coords	startway,
							endway;
size_t *ways;

size_t numOfObstacles = 0;
size_t graphSize = 0;
size_t calculatedPoints = 0;
size_t 	target = 1,
		home = 1;


namespace bases
{
    using namespace graphbases;

	struct coords   createCoords(const COORDS_DATATYPE x, const COORDS_DATATYPE y)
	{
		struct coords ret;
		ret.x = x;
		ret.y = y;
		return ret;
	}


	struct obstacle createObstacle(const COORDS_DATATYPE x, const COORDS_DATATYPE y, const COORDS_DATATYPE a, const COORDS_DATATYPE b)
	{
		struct obstacle ret;
		ret.c = (struct coords*)malloc(sizeof(struct coords));
		ret.c->x = x;
		ret.c->y = y;
		ret.a    = a;
		ret.b    = b;
		for(uint8_t i = 0; i < 4; i++)
		{
			ret.corners[i] = (struct graphbases::graphPoint*)malloc(sizeof(struct graphbases::graphPoint));
			struct coords *c = geometry::getCoordsOfCorner(&ret, i);
			ret.corners[i]->c = *c;
			free(c);
		}

		return ret;
	}

	struct obstacle createObstacle(const COORDS_DATATYPE x, const COORDS_DATATYPE y, const COORDS_DATATYPE a)
	{
		struct obstacle ret;
		ret.c = (struct graphbases::coords*)malloc(sizeof(struct graphbases::coords));
		ret.c->x = x;
		ret.c->y = y;
		ret.a    = a;
		ret.b    = a;
		for(uint8_t i = 0; i < 4; i++)
		{
			ret.corners[i] = (struct graphbases::graphPoint*)malloc(sizeof(struct graphbases::graphPoint));
			struct coords *c = geometry::getCoordsOfCorner(&ret, i);
			ret.corners[i]->c = *c;
			free(c);
		}

		return ret;
	}




    void            addTarget(struct graphbases::graphPoint *start, struct graphbases::graphPoint *end)
    {
        if(start == end) 
        {
            return;
        }
        for(size_t i = 0; i < start->numOfTargets; i++)
            if(start->targets[i] == end)
            {
                return;
            }
        for(size_t i = 0; i < end->numOfTargets; i++)
            if(end->targets[i] == start)
            {
                return;
            }


        start->targets = (struct graphbases::graphPoint**)realloc(start->targets, (++(start->numOfTargets)) * sizeof(struct graphbases::graphPoint*));
        start->targets[start->numOfTargets - 1] = end;

        end->targets = (struct graphbases::graphPoint**)realloc(end->targets, (++(end->numOfTargets)) * sizeof(struct graphbases::graphPoint*));
        end->targets[end->numOfTargets - 1] = start;
    }
}
