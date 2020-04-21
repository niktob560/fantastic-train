#pragma once

#include "bases.hpp"
#include "grapher.hpp"
#include "geometry.hpp"
#include "pathfinder.hpp"
#include "manager.hpp"

namespace fantastictrain 
{
    using graphbases::COORDS_DATATYPE;
    using graphbases::coords;

    void run(void);

    void init(void);

    uint8_t* getWaysVector(void);

    inline uint8_t getGraphSize(void)
    {
        return bases::graphSize;
    }

    inline uint8_t getNumOfObstacles(void)
    {
        return bases::numOfObstacles;
    }

    void setHomeCoords(const coords c);
    
    void setTargetCoords(const coords c);

    void addObstacle(const coords c, const COORDS_DATATYPE a, const COORDS_DATATYPE b, const uint8_t rotation);

} //namespace fantastictrain