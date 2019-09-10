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

    struct baseline b;

    b.k = INF;
    b.c = obst->c->x;
    c1 = getIntersection(&b, &vectToBaseline(v));


    b.k = INF;
    b.c = obst->c->x + obst->a;
    c2 = getIntersection(*b, &vectToBaseline(v));


    if(!isDotInside(&c1, obst) && isDotInside(&c2, obst))
        c1 = c2;
    //have one intersection in c1 at here

    b.k = 0;
    b.c = obst->c->y;
    c2 = getIntersection(&b, &vectToBaseline(v));


    b.k = 0;
    b.c = obst->c->y + obst->a;
    c3 = getIntersection(&b, &vectToBaseline(v));

    if(!isDotInside(&c3, obst) && isDotInside(&c4, obst))
        c2 = c3;

    if(!isDotInside(&c1, obst))
        if(!isDotInside(&c2, obst))
            return NULL;        //no intersections
        else
            return c2;          //intersection only in c2
    else
        if(!isDotInside(&c2, obst))
            return c1;          //intersection only in c1



    COORDS_DATATYPE l1 = getLen(&createCoords(v->x, v->y), &c1),
                    l2 = getLen(&createCoords(v->x, v->y), &c3);

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

struct coords   createCoords(COORDS_DATATYPE x, COORDS_DATATYPE y)
{
    struct coords ret;
    ret.x = x;
    ret.y = y;
    return ret;
}

struct obstacle createObstacle(COORDS_DATATYPE x, COORDS_DATATYPE y, COORDS_DATATYPE a)
{
    struct obstacle ret;
    ret.c = (struct coords*)malloc(sizeof(struct coords));
    ret.c->x = x;
    ret.c->y = y;
    ret.a    = a;
    return ret;
}
