#pragma once

#include <stdint.h>
#include "bases.hpp"
#include <stdint.h>

namespace geometry
{
    using namespace bases;
    using namespace graphbases;
    const uint16_t FIELD_MAX_LEN = 2000;

    extern struct coords homeCoords, targetCoords;



    struct vect createVect(const struct coords *c, const COORDS_DATATYPE angle);

    struct vect createVect(const struct coords *start, const struct coords *end);

    struct vect createVect(const struct obstacle *obst, const Corner corner, const COORDS_DATATYPE angle);

    bool hasIntersections(const struct vect *v);

    bool isDotInside(const struct coords *c);

    struct coords getCoordsOfPoint(struct graphbases::graphPoint *p);

    struct graphbases::graphPoint* getPoint(const struct obstacle *obst, const Corner corner);

    uint16_t getLen(const struct vect *v);


    bool            hasIntersection(const struct baseline *b1,  const struct baseline *b2);

    bool            hasIntersection(const struct obstacle *o, const struct vect *v);

    struct coords   getIntersection(const struct baseline *b1, const struct baseline *b2);

    struct coords   getIntersection(const struct obstacle *obst, const struct vect *v);

    bool            isDotInside(const struct coords *dot, const struct obstacle *obstacle);

    struct baseline vectToBaseline(const struct vect *v);

    uint16_t getLen(const struct coords *c1, const struct coords *c2);

    bool hasIntersection(const struct vect *v1, const struct vect *v2);

    struct coords getIntersection(const struct vect *v1, const struct vect *v2);

    struct coords  getCoordsOfCorner(const struct obstacle *obst, const Corner corner);

}
