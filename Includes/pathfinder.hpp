#include "bases.hpp"

namespace pathfinder
{
    using namespace bases;
    
    bool hasUncalculatedPoints();

    struct graphPoint* getMinTarget();

    void calculateWay(size_t index);

    COORDS_DATATYPE getWayPrice(struct graphPoint *start, struct graphPoint *end);

    COORDS_DATATYPE getWayPrice(struct coords *start, struct coords *end);
}