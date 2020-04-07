#include "grapher.hpp"

namespace grapher
{
	using namespace bases;
    using namespace graphbases;
    using graphbases::graphPoint;


	uint8_t getMinDatasetSize()
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
		uint16_t watchRadius = ZERO_QUAD_CHECK_SIDE;
		while (size < getMinDatasetSize()) {
			size = 0;
			for(size_t i = 0; i < graphSize; i++)
			{
				if(graph[i]->c.x >= c->x - watchRadius
				&& graph[i]->c.x <= c->x + watchRadius
				&& graph[i]->c.y >= c->y - watchRadius
				&& graph[i]->c.y <= c->y + watchRadius)
					size++;
			}
			if(size < getMinDatasetSize())
			{
				watchRadius += 100;
			}
		}
		size_t iter = 0;
		struct graphPoint **dataset = (struct graphPoint**)malloc(sizeof(struct graphPoint*) * size);
		for(size_t i = 0; i < graphSize; i++)
		{
			if(graph[i]->c.x >= c->x - watchRadius
			&& graph[i]->c.x <= c->x + watchRadius
			&& graph[i]->c.y >= c->y - watchRadius
			&& graph[i]->c.y <= c->y + watchRadius)
				dataset[iter++] = graph[i];
		}
		struct graphbases::array *ret = (struct graphbases::array*)malloc(sizeof(struct graphbases::array));
		ret->items = dataset;
		ret->size = size;
		return ret;
	}


	void initPoint(struct graphbases::graphPoint *p)
	{
		struct graphbases::array* points = getPointsDataSet(&p->c);
		struct vect v;
		struct graphbases::graphPoint *currP = 0x00;
		for(size_t i = 0; i < points->size; i++)
		{
			currP = ((struct graphbases::graphPoint**)(points->items))[i];
			v = geometry::createVect(&p->c, &currP->c);
			if((v.dx != 0 || v.dy != 0) && !geometry::hasIntersections(&v) && !geometry::isDotInside(&p->c) && !geometry::isDotInside(&currP->c))
			{
				if(p->numOfTargets == 0)
					calculatedPoints++;
				if(currP->numOfTargets == 0)
					calculatedPoints++;
				bases::addTarget(p, currP);
				if(currP->numOfTargets == 1)
					initPoint(currP);
			}
		}
		free(points);
	}


}
