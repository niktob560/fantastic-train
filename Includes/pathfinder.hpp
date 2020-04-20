#include "bases.hpp"
#include "math.hpp"

namespace pathfinder
{
    using namespace graphbases;
    using namespace bases;
    
    bool hasUncalculatedPoints();

    struct graphbases::graphPoint* getMinTarget();

    void calculateWay(uint8_t index);

    uint16_t getWayPrice(struct graphbases::graphPoint *start, struct graphbases::graphPoint *end);

    uint16_t getWayPrice(struct coords *start, struct coords *end);
}
