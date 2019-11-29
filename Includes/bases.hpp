#pragma once

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include "math.hpp"
#include "graphbases.hpp"

namespace bases
{
    typedef float COORDS_DATATYPE;


    const uint8_t CORNER_LEFT_BOT  = 0;
    const uint8_t CORNER_LEFT_TOP  = 1;
    const uint8_t CORNER_RIGHT_TOP = 2;
    const uint8_t CORNER_RIGHT_BOT = 3;


	
    /*
    *  1-------2
    *  |       |
    *  |       |
    *  |       |
    *  0-------3
    */

    struct obstacle
    {
        struct graphbases::coords      *c = NULL;
        struct graphbases::graphPoint  *corners[4] = {NULL, NULL, NULL, NULL};
        COORDS_DATATYPE     a = 0, b = 0;
    };

    struct baseline
    {
        float               k = 0;
        COORDS_DATATYPE     b = 0;
    };

    struct vect
    {
        struct graphbases::coords      *c = NULL;
        COORDS_DATATYPE     dx = 0,
                            dy = 0;

        struct graphbases::coords getLastCoords()
        {
            struct graphbases::coords _c;
            _c.x = c->x + dx;
            _c.y = c->y + dy;
            return _c;
        }
    };


    struct obstacle createObstacle(const COORDS_DATATYPE x, const COORDS_DATATYPE y, const COORDS_DATATYPE a, const COORDS_DATATYPE b);

    struct obstacle createObstacle(const COORDS_DATATYPE x, const COORDS_DATATYPE y, const COORDS_DATATYPE a);

    struct graphbases::coords   createCoords(const COORDS_DATATYPE x, const COORDS_DATATYPE y);

    void            addTarget(struct graphbases::graphPoint *start, struct graphbases::graphPoint *end);
}

extern struct graphbases::graphPoint **graph;
extern struct bases::obstacle *obstacles;
extern size_t numOfObstacles;
extern size_t calculatedPoints;

extern struct graphbases::coords	startway,
							endway;
extern size_t *ways;

extern size_t numOfObstacles;
extern size_t graphSize;
extern size_t calculatedPoints;

extern size_t 	target,
				home;
