#include <iostream>
#include "findwayBases.hpp"

bool            hasIntersection( const struct baseline *b1,  const struct baseline *b2)
{
    return b1->k != b2->k;
}

struct coords   getIntersection(const struct baseline *b1, const struct baseline *b2)
{
    struct coords ret;

    ret.x = (b2->b - b1->b) / (b1->k - b2->k);
    ret.y = (b2->k * b1->b - b1->k * b2->b) / (b2->k - b1->k);

    return ret;
}

//will return NULL if there is no intersections
struct coords   getIntersection(const struct vect *v, const struct obstacle *obst)
{
    struct coords   c1,
                    c2,
                    c3;

    struct baseline b,
                    b2;

    b.k = INF;
    b.b = obst->c->x;
    b2 = vectToBaseline(v);
    c1 = getIntersection(&b, &b2);


    b.k = INF;
    b.b = obst->c->x + obst->a;
    c2 = getIntersection(&b, &b2);


    if(!isDotInside(&c1, obst) && isDotInside(&c2, obst))
        c1 = c2;
    //have one intersection in c1 at here

    b.k = 0;
    b.b = obst->c->y;
    c2 = getIntersection(&b, &b2);


    b.k = 0;
    b.b = obst->c->y + obst->a;
    c3 = getIntersection(&b, &b2);

    if(!isDotInside(&c2, obst) && isDotInside(&c3, obst))
        c2 = c3;

    if(!isDotInside(&c1, obst))
        if(!isDotInside(&c2, obst))
            return {-1, -1};        //no intersections
        else
            return c2;          //intersection only in c2
    else
        if(!isDotInside(&c2, obst))
            return c1;          //intersection only in c1


    c3 = createCoords(v->c->x, v->c->y);

    COORDS_DATATYPE l1 = getLen(&c3, &c1),
                    l2 = getLen(&c3, &c2);

    if(l1 > l2)
        return c1;
    else
        return c2;
}




bool            isDotInside(const struct coords *dot, const struct obstacle *obstacle)
{
    return      (dot->x >= obstacle->c->x)
            &&  (dot->x <= obstacle->c->x + obstacle->a)
            &&  (dot->y >= obstacle->c->y)
            &&  (dot->y <= obstacle->c->y + obstacle->a);
}

struct baseline vectToBaseline(const struct vect *v)
{
    struct baseline ret;

    ret.k = v->dy / v->dx;
    ret.b = v->c->y;

    return ret;
}

COORDS_DATATYPE getLen(const struct coords *c1, const struct coords *c2)
{
    COORDS_DATATYPE l = sqrt(sqr(c1->x - c2->x) + sqr(c1->y - c2->y));
    return l;
}

struct coords   createCoords(const COORDS_DATATYPE x, const COORDS_DATATYPE y)
{
    struct coords ret;
    ret.x = x;
    ret.y = y;
    return ret;
}

struct obstacle createObstacle(const COORDS_DATATYPE x, const COORDS_DATATYPE y, const COORDS_DATATYPE a)
{
    struct obstacle ret;
    ret.c = (struct coords*)malloc(sizeof(struct coords));
    ret.c->x = x;
    ret.c->y = y;
    ret.a    = a;
    return ret;
}


struct coords*  getCoordsOfCorner(const struct obstacle *obst, const uint8_t corner)
{
    struct coords *ret = (struct coords*)malloc(sizeof(struct coords));
    switch (corner) {
        case CORNER_LEFT_BOT:
        {
            ret->x = obst->c->x;
            ret->y = obst->c->y;
            break;
        }
        case CORNER_LEFT_TOP:
        {
            ret->x = obst->c->x;
            ret->y = obst->c->y + obst->a;
            break;
        }
        case CORNER_RIGHT_BOT:
        {
            ret->x = obst->c->x + obst->a;
            ret->y = obst->c->y;
            break;
        }
        case CORNER_RIGHT_TOP:
        {
            ret->x = obst->c->x + obst->a;
            ret->y = obst->c->y + obst->a;
            break;
        }
    }
    return ret;
}


struct graphPoint* getPoint(const struct obstacle *obst, const uint8_t corner)
{
    return obst->corners[corner];
}
