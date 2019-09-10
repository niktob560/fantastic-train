#include "math.hpp"

#define COORDS_DATATYPE float

struct coords
{
    COORDS_DATATYPE     x,
                        y;
};

struct obstacle
{
    struct coords      *c;
    COORDS_DATATYPE     a;
};

struct baseline
{
    COORDS_DATATYPE     k,
                        b;
};

struct vect
{
    struct coords      *c;
    COORDS_DATATYPE     dx,
                        dy;
};


bool            hasIntersection( const struct baseline *b1,  const struct baseline *b2);

struct coords   getIntersection(const struct baseline *b1, const struct baseline *b2);

//will return NULL if there is no intersections
struct coords   getIntersection(const struct vect *v, const struct obstacle *obst);

bool            isDotInside(const struct coords *dot, const struct obstacle *obstacle);

struct baseline vectToBaseline(const struct vect *v);

COORDS_DATATYPE getLen(const struct coords *c1, const struct coords *c2);

struct obstacle createObstacle(COORDS_DATATYPE x, COORDS_DATATYPE y, COORDS_DATATYPE a);

struct coords   createCoords(COORDS_DATATYPE x, COORDS_DATATYPE y);
