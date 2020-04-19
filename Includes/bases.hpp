#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include "math.hpp"
#include "graphbases.hpp"
#include "geometry.hpp"

#ifndef __BASES_HPP__
#define __BASES_HPP__

namespace bases
{
    using graphbases::COORDS_DATATYPE;


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
                                        b = 0; 
        uint8_t                         rot = 0;
    };

    struct baseline
    {
        float               k = 0;
        COORDS_DATATYPE     b = 0;
    };

    struct vect
    {
        struct graphbases::coords   c = {0, 0};
        COORDS_DATATYPE             dx = 0,
                                    dy = 0;

        constexpr struct graphbases::coords getLastCoords()
        {
            return {static_cast<COORDS_DATATYPE>(c.x + dx), static_cast<COORDS_DATATYPE>(c.y + dy)};
        }
    };


    extern struct graphbases::graphPoint graph[];
    extern struct bases::obstacle obstacles[];
    extern size_t numOfObstacles;
    extern size_t calculatedPoints;

    extern struct graphbases::coords	startway,
                                        endway;
    extern size_t ways[];

    extern size_t numOfObstacles;
    extern size_t graphSize;
    extern size_t calculatedPoints;

    extern size_t 	target,
                    home;

    template<typename T>
    T max(T t1, T t2)
    {
        if(t1 > t2)
            return t1;
        else
            return t2;
    }
                    
    struct obstacle createObstacle(const COORDS_DATATYPE x, const COORDS_DATATYPE y, const COORDS_DATATYPE a, const COORDS_DATATYPE b, const uint8_t rotation);

    struct obstacle createObstacle(const COORDS_DATATYPE x, const COORDS_DATATYPE y, const COORDS_DATATYPE a, const COORDS_DATATYPE b, const float rotation);

    struct obstacle createObstacle(const COORDS_DATATYPE x, const COORDS_DATATYPE y, const COORDS_DATATYPE a, const COORDS_DATATYPE b);

    struct obstacle createObstacle(const COORDS_DATATYPE x, const COORDS_DATATYPE y, const COORDS_DATATYPE a);

    struct graphbases::coords   createCoords(const COORDS_DATATYPE x, const COORDS_DATATYPE y);

    void            addTarget(struct graphbases::graphPoint *start, struct graphbases::graphPoint *end);
}

#endif //ifndef __BASES_HPP__