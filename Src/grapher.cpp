#include "grapher.hpp"

namespace grapher
{
	using namespace bases;
    using namespace graphbases;
    using graphbases::graphPoint;


	size_t getMinDatasetSize()
	{
		size_t ret = numOfObstacles;
		if(ret < 2)
			ret = 2;
		if(ret >= numOfObstacles)
			ret = (numOfObstacles) - 1;
		return ret;
	}

	struct graphbases::array* getPointsDataSet(struct graphbases::coords *c)
	{
		size_t size = 0;
		COORDS_DATATYPE watchRadius = ZERO_QUAD_CHECK_SIDE;
		struct coords pc;
		while (size < getMinDatasetSize()) {
			size = 0;
			for(size_t i = 0; i < graphSize; i++)
			{
				pc = geometry::getCoordsOfPoint(graph[i]);
				if(pc.x >= c->x - watchRadius
				&& pc.x <= c->x + watchRadius
				&& pc.y >= c->y - watchRadius
				&& pc.y <= c->y + watchRadius)
					size++;
			}
			if(size < getMinDatasetSize())
			{
				watchRadius = static_cast<COORDS_DATATYPE>(watchRadius + 100);
			}
		}
		size_t iter = 0;
		struct graphPoint **dataset = static_cast<struct graphPoint**>(malloc(sizeof(struct graphPoint*) * size));
		for(size_t i = 0; i < graphSize; i++)
		{
			pc = geometry::getCoordsOfPoint(graph[i]);
			if(pc.x >= c->x - watchRadius
			&& pc.x <= c->x + watchRadius
			&& pc.y >= c->y - watchRadius
			&& pc.y <= c->y + watchRadius)
				dataset[iter++] = graph[i];
		}
		struct graphbases::array *ret = static_cast<struct graphbases::array*>(malloc(sizeof(struct graphbases::array)));
		// ret->items = graph;
		// ret->size = graphSize;
		ret->items = dataset;
		ret->size = size;
		return ret;
	}


	void initPoint(struct graphbases::graphPoint *p)
	{
		struct coords 	c1 = geometry::getCoordsOfPoint(p), 
						c2;
		struct graphbases::array* points = getPointsDataSet(&c1);
		struct vect v;
		struct graphbases::graphPoint *currP = 0x00;
		for(size_t i = 0; i < points->size; i++)
		{
			currP = (static_cast<struct graphbases::graphPoint**>(points->items))[i];
			// if(!currP->calculated)
			{
				c2 = geometry::getCoordsOfPoint(currP);
				v = geometry::createVect(&c1, &c2);
				//TODO: cleanup
				if((v.dx != 0 || v.dy != 0) && !geometry::hasIntersections(&v) && !geometry::isDotInside(&c1) && !geometry::isDotInside(&c2))
				{
					bool state = false;
					if(p->numOfTargets == 0) 
					{
						state = true;
						calculatedPoints++;
					}

					if(currP->numOfTargets == 0)
					{
						state = true;
						calculatedPoints++;
					}

					bases::addTarget(p, currP);
					if(state)
						initPoint(currP);
				}
				free(v.c);
			}
		}
		free(points);
	}
}


