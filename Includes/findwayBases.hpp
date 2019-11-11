#include "math.hpp"
#include <string>

#define COORDS_DATATYPE float

#define CORNER_LEFT_BOT     0
#define CORNER_LEFT_TOP     1
#define CORNER_RIGHT_TOP    2
#define CORNER_RIGHT_BOT    3


struct coords
{
    COORDS_DATATYPE     x,
                        y;
    std::string toString()
    {
        std::string str = "";
        str += "{";
        str += std::to_string(x);
        str += ", ";
        str += std::to_string(y);
        str += "}";
        return str;
    }

    bool eq(struct coords *c)
    {
        return (x == c->x && y == c->y);
    }

    bool eq(COORDS_DATATYPE _x, COORDS_DATATYPE _y)
    {
        return (x == _x && y == _y);
    }
};

struct graphPoint
{
    struct graphPoint **targets = NULL;
    size_t numOfTargets = 0;
    struct coords c;
    bool calculated = false;
    COORDS_DATATYPE weight = INF;
    size_t i = 0;

    std::string toString()
    {
        std::string str = "";
        str += "{tgts: ";
        str += std::to_string(numOfTargets);
        str += "; cal: ";
        str += std::to_string(calculated);
        str += "; weight: ";
        str += std::to_string(weight);
        str += "; i: ";
        str += std::to_string(i);
        str += "}";
        return str;
    }
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
    struct coords      *c = NULL;
    struct graphPoint  *corners[4] = {NULL, NULL, NULL, NULL};
    COORDS_DATATYPE     a = 0, b = 0;
};

struct baseline
{
    float               k = 0;
    COORDS_DATATYPE     b = 0;
};

struct vect
{
    struct coords      *c = NULL;
    COORDS_DATATYPE     dx = 0,
                        dy = 0;

    std::string toString()
    {
        std::string str = "";
        str += "{";
        str += std::to_string(dx);
        str += ", ";
        str += std::to_string(dy);
        str += "}";
        return str;
    }

    struct coords getLastCoords()
    {
        struct coords _c;
        _c.x = c->x + dx;
        _c.y = c->y + dy;
        return _c;
    }
};


bool            hasIntersection(const struct baseline *b1,  const struct baseline *b2);

bool            hasIntersection(const struct obstacle *o, const struct vect *v);

struct coords   getIntersection(const struct baseline *b1, const struct baseline *b2);

//will return NULL if there is no intersections
struct coords   getIntersection(const struct obstacle *obst, const struct vect *v);

bool            isDotInside(const struct coords *dot, const struct obstacle *obstacle);

struct baseline vectToBaseline(const struct vect *v);

COORDS_DATATYPE getLen(const struct coords *c1, const struct coords *c2);

struct obstacle createObstacle(const COORDS_DATATYPE x, const COORDS_DATATYPE y, const COORDS_DATATYPE a, const COORDS_DATATYPE b);

struct obstacle createObstacle(const COORDS_DATATYPE x, const COORDS_DATATYPE y, const COORDS_DATATYPE a);

struct coords   createCoords(const COORDS_DATATYPE x, const COORDS_DATATYPE y);

struct coords*  getCoordsOfCorner(const struct obstacle *obst, const uint8_t corner);

struct graphPoint* getPoint(const struct obstacle *obst, const uint8_t corner);

void            addTarget(struct graphPoint *start, struct graphPoint *end);

COORDS_DATATYPE getLen(const struct vect *v);

COORDS_DATATYPE getWayPrice(struct graphPoint *start, struct graphPoint *end);

COORDS_DATATYPE getWayPrice(struct coords *start, struct coords *end);