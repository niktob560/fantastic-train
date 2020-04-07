#include "bases.hpp"
#include "bases.hpp"
#include "math.hpp"

namespace pathfinder
{
    using namespace graphbases;
    using namespace bases;
    
    bool hasUncalculatedPoints();

    struct graphbases::graphPoint* getMinTarget();

    void calculateWay(size_t index);

    COORDS_DATATYPE getWayPrice(struct graphbases::graphPoint *start, struct graphbases::graphPoint *end);

    COORDS_DATATYPE getWayPrice(struct coords *start, struct coords *end);
}
