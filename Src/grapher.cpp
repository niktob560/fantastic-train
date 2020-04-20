#include "grapher.hpp"

namespace grapher
{
	using namespace bases;
    using namespace graphbases;
    using graphbases::graphPoint;



	void initPoint(struct graphbases::graphPoint *p)
	{
		struct coords 	c1 = geometry::getCoordsOfPoint(p), 
						c2;
		struct vect v;
		struct graphbases::graphPoint *currP = 0x00;
		for(size_t i = 0; i < graphSize; i++)
		{
			currP = &graph[i];
			c2 = geometry::getCoordsOfPoint(currP);
			v = geometry::createVect(&c1, &c2);

			if((v.dx != 0 || v.dy != 0) && !geometry::hasIntersections(&v))
			{
				bool wannaInitThisPoint = false;
				if(p->numOfTargets == 0) 
				{
					wannaInitThisPoint = true;
					calculatedPoints++;
				}

				if(currP->numOfTargets == 0)
				{
					wannaInitThisPoint = true;
					calculatedPoints++;
				}

				bases::addTarget(p, currP);
				if(wannaInitThisPoint)
					initPoint(currP);
			}
		}
	}
}


