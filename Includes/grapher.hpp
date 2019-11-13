#pragma once

#include <unistd.h>
#include <stdint.h>
#include "bases.hpp"

namespace grapher
{
	using namespace bases;

    const uint16_t ZERO_QUAD_CHECK_SIDE = 2000;



    uint8_t getMinDatasetSize();

    struct array* getPointsDataSet(const struct coords *c);

    void initPoint(struct graphPoint *p);
}