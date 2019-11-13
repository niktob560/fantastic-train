#pragma once

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include "math.hpp"

namespace bases
{
    typedef float COORDS_DATATYPE;


    const uint8_t CORNER_LEFT_BOT  = 0;
    const uint8_t CORNER_LEFT_TOP  = 1;
    const uint8_t CORNER_RIGHT_TOP = 2;
    const uint8_t CORNER_RIGHT_BOT = 3;
 
	struct array
	{
		void *items;
		size_t size;
	};


	struct coords
	{
		COORDS_DATATYPE     x,
							y;

		bool eq(struct coords *c)
		{
			return (x == c->x && y == c->y);
		}

		bool eq(COORDS_DATATYPE _x, COORDS_DATATYPE _y)
		{
			return (x == _x && y == _y);
		}
	};



    struct graphPoint
    {
        struct graphPoint **targets = NULL;
        size_t numOfTargets = 0;
        struct bases::coords c;
        bool calculated = false;
        COORDS_DATATYPE weight = INF;
        size_t i = 0;
    };

    /*
    *  1-------2
    *  |       |
    *  |       |
    *  |       |
    *  0-------3
    */

    struct obstacle
    {
        struct bases::coords      *c = NULL;
        struct graphPoint  *corners[4] = {NULL, NULL, NULL, NULL};
        COORDS_DATATYPE     a = 0, b = 0;
    };

    struct baseline
    {
        float               k = 0;
        COORDS_DATATYPE     b = 0;
    };

    struct vect
    {
        struct bases::coords      *c = NULL;
        COORDS_DATATYPE     dx = 0,
                            dy = 0;

        struct bases::coords getLastCoords()
        {
            struct bases::coords _c;
            _c.x = c->x + dx;
            _c.y = c->y + dy;
            return _c;
        }
    };


    struct obstacle createObstacle(const COORDS_DATATYPE x, const COORDS_DATATYPE y, const COORDS_DATATYPE a, const COORDS_DATATYPE b);

    struct obstacle createObstacle(const COORDS_DATATYPE x, const COORDS_DATATYPE y, const COORDS_DATATYPE a);

    struct bases::coords   createCoords(const COORDS_DATATYPE x, const COORDS_DATATYPE y);

    void            addTarget(struct graphPoint *start, struct graphPoint *end);
}

extern struct bases::graphPoint **graph;
extern struct bases::obstacle *obstacles;
extern size_t numOfObstacles;
extern size_t calculatedPoints;

extern struct bases::coords	startway,
							endway;
extern size_t *ways;

extern size_t numOfObstacles;
extern size_t graphSize;
extern size_t calculatedPoints;

extern size_t 	target,
				home;
