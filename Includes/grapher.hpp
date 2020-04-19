#pragma once

#include <unistd.h>
#include <stdint.h>
#include "bases.hpp"
#include "geometry.hpp"

namespace grapher
{
	using namespace bases;

    const uint16_t ZERO_QUAD_CHECK_SIDE = 2000;



    size_t getMinDatasetSize();

    struct graphbases::array* getPointsDataSet(const struct graphbases::coords *c);

    void initPoint(struct graphbases::graphPoint *p);
}
