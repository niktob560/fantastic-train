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



    enum Corner
    {
        LEFT_BOTTOM     = 0,
        LEFT_TOP        = 1,
        RIGHT_TOP       = 2,
        RIGHT_BOTTOM    = 3
    };

    template<typename T>
    constexpr Corner cornerFromNum(const T num)
    {
        Corner ret;
        switch (num)
        {
        case 0:
            ret = LEFT_BOTTOM;
            break;
        case 1:
            ret = LEFT_TOP;
            break;
        case 2:
            ret = RIGHT_TOP;
            break;
        case 3:
            ret = RIGHT_BOTTOM;
            break;
        default:
            ret = LEFT_BOTTOM;
        }
        return ret;
    }


	
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
        COORDS_DATATYPE                 a = 0, 
                                        b = 0, 
                                        rot = 0;
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

                    
    struct obstacle createObstacle(const COORDS_DATATYPE x, const COORDS_DATATYPE y, const COORDS_DATATYPE a, const COORDS_DATATYPE b, const COORDS_DATATYPE rotation);

    struct obstacle createObstacle(const COORDS_DATATYPE x, const COORDS_DATATYPE y, const COORDS_DATATYPE a, const COORDS_DATATYPE b);

    struct obstacle createObstacle(const COORDS_DATATYPE x, const COORDS_DATATYPE y, const COORDS_DATATYPE a);

    struct graphbases::coords   createCoords(const COORDS_DATATYPE x, const COORDS_DATATYPE y);

    void            addTarget(struct graphbases::graphPoint *start, struct graphbases::graphPoint *end);
}
