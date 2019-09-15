#include <iostream>
#include "findwayBases.hpp"

bool            hasIntersection( const struct baseline *b1,  const struct baseline *b2)
{
    return b1->k != b2->k;
}

struct coords   getIntersection(const struct baseline *b1, const struct baseline *b2)
{
    struct coords ret;

    if(b1->k != INF && b2->k != INF)
    {
        ret.x = (b2->b - b1->b) / (b1->k - b2->k);
        ret.y = (b2->k * b1->b - b1->k * b2->b) / (b2->k - b1->k);
    }
    else if(b1->k == INF)
    {
        ret.x = b1->b;
        ret.y = b2->k * ret.x + b2->b;
    }
    else
    {
        ret.x = b2->b;
        ret.y = b1->k * ret.x + b1->b;
    }

    return ret;
}

//will return NULL if there is no intersections
struct coords   getIntersection(const struct obstacle *obst, const struct vect *v)
{
    // if((obst->c->x - v->c->x) / v->dx < 0 || (obst->c->y - v->c->y) / v->dy < 0)
    // if(!
    //     (
    //         (
    //             (
    //                 v->dx >= 0
    //              && v->c->x <= obst->c->x
    //              && obst->c->x + obst->a <= v->c->x + v->dx
    //             )
    //             ||
    //             (
    //                 v->dx < 0
    //              && v->c->x >= obst->c->x + obst->a
    //              && obst->c->x + obst->a >= v->c->x + v->dx
    //             )
    //         )
    //         &&
    //         (
    //             (
    //                 v->dy >= 0
    //              && v->c->y <= obst->c->y
    //              && obst->c->y + obst->a <= v->c->y + v->dy
    //             )
    //             ||
    //             (
    //                 v->dy < 0
    //              && v->c->y >= obst->c->y + obst->a
    //              && obst->c->y + obst->a  >= v->c->y + v->dy
    //              )
    //          )
    //      )
    //  )
    // std::cout << "   " << (       v->dx >= 0          ) << "\n";
    if(!
        (
            (
                (
                       v->dx >= 0
                    && v->c->x <= obst->c->x + obst->a
                    && obst->c->x <= v->c->x + v->dx
                )
                ||
                (
                       v->dx < 0
                    && v->c->x >= obst->c->x
                    && obst->c->x >= v->c->x + v->dx
                )
            )
            &&
            (
                (
                       v->dy >= 0
                    && v->c->y <= obst->c->y + obst->a
                    && obst->c->y <= v->c->y + v->dy
                )
                ||
                (
                       v->dy < 0
                    && v->c->y >= obst->c->y
                    && obst->c->y >= v->c->y + v->dy
                )
            )
        )
    )
    {
        return {-1, -1};
    }

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
    {
        c1 = c2;
    }
    //have one intersection in c1 at here

    b.k = 0;
    b.b = obst->c->y;
    c2 = getIntersection(&b, &b2);


    b.k = 0;
    b.b = obst->c->y + obst->a;
    c3 = getIntersection(&b, &b2);

    if(!isDotInside(&c2, obst) && isDotInside(&c3, obst))
    {
        c2 = c3;
    }



    c3 = createCoords(v->c->x, v->c->y);

    COORDS_DATATYPE l1 = getLen(&c3, &c1),
                    l2 = getLen(&c3, &c2),
                    l3 = getLen(v);

    std::cout << "l1: " << l1 << "; l2: " << l2 << "; l3: " << l3 << std::endl;

    if(l3 < l2 && l3 < l1)
        return {-1, -1};

    if(!isDotInside(&c1, obst))
        if(!isDotInside(&c2, obst))
            return {-1, -1};        //no intersections
        else
            return c2;          //intersection only in c2
    else
        if(!isDotInside(&c2, obst))
            return c1;          //intersection only in c1


    if(l1 > l2 && l3 >= l1)
        return c1;
    else if(l3 >= l2)
        return c2;
    else
        return {-1, -1};
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
    ret.b = v->c->y - v->c->x * ret.k;

    return ret;
}

COORDS_DATATYPE getLen(const struct coords *c1, const struct coords *c2)
{
    COORDS_DATATYPE lx = c1->x - c2->x,
                    ly = c1->y - c2->y,
                    l  = sqrt(sqr(lx) + sqr(ly));

    return l;
}

COORDS_DATATYPE getLen(const struct vect *v)
{
    COORDS_DATATYPE l = sqrt(sqr(v->dx) + sqr(v->dy));
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
    for(uint8_t i = 0; i < 4; i++)
    {
        ret.corners[i] = (struct graphPoint*)malloc(sizeof(struct graphPoint));
        ret.corners[i]->c = *getCoordsOfCorner(&ret, i);
    }
    return ret;
}


#define CONST 0.4

struct coords*  getCoordsOfCorner(const struct obstacle *obst, const uint8_t corner)
{
    struct coords *ret = (struct coords*)malloc(sizeof(struct coords));
    switch (corner) {
        case CORNER_LEFT_BOT:
        {
            ret->x = obst->c->x - CONST;
            ret->y = obst->c->y - CONST;
            break;
        }
        case CORNER_LEFT_TOP:
        {
            ret->x = obst->c->x - CONST;
            ret->y = obst->c->y + obst->a + CONST;
            break;
        }
        case CORNER_RIGHT_BOT:
        {
            ret->x = obst->c->x + obst->a + CONST;
            ret->y = obst->c->y - CONST;
            break;
        }
        case CORNER_RIGHT_TOP:
        {
            ret->x = obst->c->x + obst->a + CONST;
            ret->y = obst->c->y + obst->a + CONST;
            break;
        }
    }
    return ret;
}


struct graphPoint* getPoint(const struct obstacle *obst, const uint8_t corner)
{
    return obst->corners[corner];
}



bool            hasIntersection(const struct obstacle *obst, const struct vect *v)
{
    // if((obst->c->x - v->c->x) / v->dx < 0 || (obst->c->y - v->c->y) / v->dy < 0)
    //     return false;
    //
    // struct coords   c1,
    //                 c2,
    //                 c3;
    //
    // struct baseline b,
    //                 b2;
    //
    // b.k = INF;
    // b.b = obst->c->x;
    // b2 = vectToBaseline(v);
    // c1 = getIntersection(&b, &b2);
    //
    //
    // b.k = INF;
    // b.b = obst->c->x + obst->a;
    // c2 = getIntersection(&b, &b2);
    //
    //
    // if(!isDotInside(&c1, obst) && isDotInside(&c2, obst))
    // {
    //     std::cout << "c1 not inside\n";
    //     c1 = c2;
    // }
    // //have one intersection in c1 at here
    //
    // b.k = 0;
    // b.b = obst->c->y;
    // c2 = getIntersection(&b, &b2);
    //
    //
    // b.k = 0;
    // b.b = obst->c->y + obst->a;
    // c3 = getIntersection(&b, &b2);
    //
    // if(!isDotInside(&c2, obst) && isDotInside(&c3, obst))
    // {
    //     std::cout << "c2 not inside\n";
    //     c2 = c3;
    // }
    //
    // if(!isDotInside(&c1, obst)
    // && !isDotInside(&c2, obst))
    //     return false;
    //
    // return true;
    struct coords c = getIntersection(obst, v);
    std::cout << "obst: " << obst->c->x << ":" << obst->c->y << "; vect: " << v->c->x << ":" << v->c->y;
    if(c.x == -1 && c.y == -1)
    {
        std::cout << "have no intersection" << std::endl;
        return false;
    }
    else
    {
        std::cout << "have intersection" << std::endl;
        return true;
    }
}


void            addTarget(struct graphPoint *start, struct graphPoint *end)
{
    start->targets = (struct graphPoint**)realloc(start->targets, (++(start->numOfTargets)) * sizeof(struct graphPoint*));
    start->targets[start->numOfTargets - 1] = end;

    end->targets = (struct graphPoint**)realloc(end->targets, (++(end->numOfTargets)) * sizeof(struct graphPoint*));
    end->targets[end->numOfTargets - 1] = start;
}
