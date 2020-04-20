#include "bases.hpp"

namespace bases
{
    using namespace graphbases;

	struct graphbases::graphPoint graph[200];
	struct bases::obstacle obstacles[50];

	struct graphbases::coords	startway,
								endway;
	uint8_t	ways[200];

	uint8_t	numOfObstacles 		= 0;
	uint8_t	graphSize 			= 0;
	uint8_t	calculatedPoints 	= 0;


    struct obstacle createObstacle(const COORDS_DATATYPE x, const COORDS_DATATYPE y, const COORDS_DATATYPE a, const COORDS_DATATYPE b, const uint8_t rotation)
	{
		struct obstacle ret;
		ret.c = {x, y};
		ret.a    = a;
		ret.b    = b;
		ret.rot  = rotation;
		for(uint8_t i = 0; i < 4; i++)
		{
			ret.corners[i] = &graph[graphSize];
			ret.corners[i]->i = graphSize;
			graphSize++;
		}

		return ret;
	}


    struct obstacle createObstacle(const COORDS_DATATYPE x, const COORDS_DATATYPE y, const COORDS_DATATYPE a, const COORDS_DATATYPE b, const float rotation)
	{
		return createObstacle(x, y, a, b, static_cast<uint8_t>((rotation * 180) / M_PI));
	}

	struct obstacle createObstacle(const COORDS_DATATYPE x, const COORDS_DATATYPE y, const COORDS_DATATYPE a, const COORDS_DATATYPE b)
	{
		return createObstacle(x, y, a, b, 0.0f);
	}

	struct obstacle createObstacle(const COORDS_DATATYPE x, const COORDS_DATATYPE y, const COORDS_DATATYPE a)
	{
		return createObstacle(x, y, a, a);
	}




    void            addTarget(struct graphbases::graphPoint *start, struct graphbases::graphPoint *end)
    {
        if(start == end) 
            return;
        
        for(size_t i = 0; i < start->numOfTargets; i++)
		{
            if(start->targets[i] == end)
			{
                return;
			}
		}
            
        for(size_t i = 0; i < end->numOfTargets; i++)
		{
            if(end->targets[i] == start)
			{
                return;
			}
		}
            

		if(start->numOfTargets < sizeof(*graphbases::graphPoint::targets))
		{
        	start->targets[start->numOfTargets] = end;
			start->numOfTargets++;
		}

		if(end->numOfTargets < sizeof(*graphbases::graphPoint::targets))
		{
        	end->targets[end->numOfTargets] = start;
			end->numOfTargets++;
		}
    }
}
