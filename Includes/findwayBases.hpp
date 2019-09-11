#include "math.hpp"

#define COORDS_DATATYPE float

#define CORNER_LEFT_BOT     0
#define CORNER_LEFT_TOP     1
#define CORNER_RIGHT_TOP    2
#define CORNER_RIGHT_BOT    3


struct graphPoint
{
    struct graphPoint **targets;
};

struct coords
{
    COORDS_DATATYPE     x,
                        y;
};

/*
 *  1-------2
 *  |       |
 *  |       |
 *  |       |
 *  0-------3
 */

struct obstacle
{
    struct coords      *c;
    struct graphPoint  *corners[4];
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

struct obstacle createObstacle(const COORDS_DATATYPE x, const COORDS_DATATYPE y, const COORDS_DATATYPE a);

struct coords   createCoords(const COORDS_DATATYPE x, const COORDS_DATATYPE y);

struct coords*  getCoordsOfCorner(const struct obstacle *obst, const uint8_t corner);

struct graphPoint* getPoint(const struct obstacle *obst, const uint8_t corner);
