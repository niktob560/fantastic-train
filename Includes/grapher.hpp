#pragma once

#include <unistd.h>
#include <stdint.h>
#include "bases.hpp"
#include "geometry.hpp"

namespace grapher
{
	using namespace bases;

    const uint16_t ZERO_QUAD_CHECK_SIDE = 2000;



    void initPoint(struct graphbases::graphPoint *p);
}
