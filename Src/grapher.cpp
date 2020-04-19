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
		}
	}
}


