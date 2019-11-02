#include <iostream>
#include "findwayBases.hpp"

bool            hasIntersection( const struct baseline *b1,  const struct baseline *b2)
{
    return b1->k != b2->k;
}

struct coords   getIntersection(const struct baseline *b1, const struct baseline *b2)
{
    struct coords ret;
    if(b1->k == b2->k)
        ret = {0, 0};
    else 
    {
        if(abs(b1->k) != INF && abs(b2->k) != INF)
        {
            ret.x = (b2->b - b1->b) / (b1->k - b2->k);
            ret.y = (b2->k * b1->b - b1->k * b2->b) / (b2->k - b1->k);
        }
        else if(abs(b1->k) == INF)
        {
            ret.x = b1->b;
            ret.y = b2->k * ret.x + b2->b;
        }
        else
        {
            ret.x = b2->b;
            ret.y = b1->k * ret.x + b1->b;
        }
    }

    return ret;
}

bool isCoDirectional(const struct vect *v, const struct coords *c)
{
    return (sgn(v->dx) == sgn(c->x - v->c->x) && sgn(v->dy) == sgn(c->y - v->c->y));
}


//will return NULL if there is no intersections
struct coords   getIntersection(const struct obstacle *obst, const struct vect *v)
{
    struct coords inter1, inter2, tgt;
    tgt = {(v->dx + v->c->x), (v->dy + v->c->y)};
    struct baseline b1,
                    b2;
    COORDS_DATATYPE l1 = 0, l2 = 0, lv = 0;

    lv = getLen(v);

    b1 = vectToBaseline(v);

    b2.k = INF;
    b2.b = obst->c->x;

    inter1 = getIntersection(&b1, &b2);
    if(isDotInside(&inter1, obst))
    {
        l1 = getLen(v->c, &inter1);
        if(!(isCoDirectional(v, &inter1) && l1 <= lv))
        {
            l1 = 0;
            inter1 = {0, 0};
        }
    }
    else
    {
        l1 = 0;
        inter1 = {0, 0};
    }

    b2.b += obst->a;

    inter2 = getIntersection(&b1, &b2);
    if(isDotInside(&inter2, obst))
    {
        l2 = getLen(v->c, &inter2);
        if(!(isCoDirectional(v, &inter2) && l2 <= lv))
        {
            inter2 = {0, 0};
            l2 = 0;
        }
        else if(inter1.eq(0, 0) || l2 < l1)
        {
            inter1 = inter2;
            l1 = l2;
            l2 = 0;
            inter2 = {0, 0};
        }
        else
        {
            l2 = 0;
            inter2 = {0, 0};
        }
    }
    else
    {
        l2 = 0;
        inter2 = {0, 0};
    }

    // if(!inter2.eq(0, 0))
    // {
    //     if(!inter1.eq(0, 0))
    //     {
    //         if(l1 <= lv)
    //         {
    //             if(l2 <= lv)
    //             {
    //                 if(l2 < l1)
    //                 {
    //                     inter1 = inter2;
    //                     l1 = l2;

    //                     l2 = 0;
    //                     inter2 = {0, 0};
    //                 }
    //             }
    //             else 
    //             {
    //                 l2 = 0;
    //                 inter2 = {0, 0};
    //             }
    //         }
    //         else
    //         {
    //             if(l2 <= lv)
    //             {
    //                 inter1 = inter2;
    //                 l1 = l2;

    //                 l2 = 0;
    //                 inter2 = {0, 0};
    //             }
    //             else
    //             {
    //                 l1 = 0;
    //                 inter1 = {0, 0};

    //                 l2 = 0;
    //                 inter2 = {0, 0};
    //             }
                
    //         }
    //     }
    //     else
    //     {
    //         if(l2 <= lv)
    //         {
    //             inter1 = inter2;
    //             l1 = l2;

    //             l2 = 0;
    //             inter2 = {0, 0};
    //         }
    //         else
    //         {
    //             inter1 = {0, 0};
    //             l1 = 0;

    //             l2 = 0;
    //             inter2 = {0, 0};
    //         }
    //     }   
    // }
    // else if(!inter1.eq(0, 0))
    // {
    //     if(l1 > lv)
    //     {
    //         inter1 = {0, 0};
    //         l1 = 0;
    //     }
    //     l2 = 0;
    //     inter2 = {0, 0};
    // }
    // else 
    // {
    //     l1 = 0;
    //     inter1 = {0, 0};

    //     l2 = 0;
    //     inter2 = {0, 0};
    // }

    b2.k = 0;
    b2.b = obst->c->y;
    inter2 = getIntersection(&b1, &b2);
    if(isDotInside(&inter2, obst))
    {
        l2 = getLen(v->c, &inter2);
        if(!(isCoDirectional(v, &inter2) && l2 <= lv))
        {
            inter2 = {0, 0};
            l2 = 0;
        }
        else if(inter1.eq(0, 0) || l2 < l1)
        {
            inter1 = inter2;
            l1 = l2;
            l2 = 0;
            inter2 = {0, 0};
        }
        else
        {
            l2 = 0;
            inter2 = {0, 0};
        }
    }
    else
    {
        l2 = 0;
        inter2 = {0, 0};
    }

    
    b2.b += obst->a;

    inter2 = getIntersection(&b1, &b2);
    if(isDotInside(&inter2, obst))
    {
        l2 = getLen(v->c, &inter2);
        if(!(isCoDirectional(v, &inter2) && l2 <= lv))
        {
            inter2 = {0, 0};
            l2 = 0;
        }
        else if(inter1.eq(0, 0) || l2 < l1)
        {
            inter1 = inter2;
            l1 = l2;
            l2 = 0;
            inter2 = {0, 0};
        }
        else
        {
            l2 = 0;
            inter2 = {0, 0};
        }
    }
    else
    {
        l2 = 0;
        inter2 = {0, 0};
    }

    return inter1;
    
    // struct coords   c,
    //                 c2,
    //                 c3;

    // struct baseline b,
    //                 b2;

    // COORDS_DATATYPE l1,
    //                 l2,
    //                 l3;
    // c3 = {(v->dx + v->c->x), (v->dy + v->c->y)};
    // l3 = getLen(v->c, &c3);

    // b.k = INF;
    // b.b = obst->c->x;
    // b2 = vectToBaseline(v);
    // c = getIntersection(&b, &b2);

    // b.k = INF;
    // b.b = obst->c->x + obst->a;
    // c2 = getIntersection(&b, &b2);
    // l1 = getLen(v->c, &c);
    // if(!isDotInside(&c, obst) && isDotInside(&c2, obst))
    // {
    //     c = c2;
    // }
    // else if(isDotInside(&c, obst) && isDotInside(&c2, obst))
    // {
    //     l2 = getLen(v->c, &c2);
    //     if(l1 < l2 && l1 <= l3)
    //         c = c2;
    // }

    // b.k = 0;
    // b.b = obst->c->y;
    // c2 = getIntersection(&b, &b2);
    // if(isDotInside(&c2, obst))
    // {
    //     l2 = getLen(v->c, &c2);
    //     if(l1 < l2 && l1 <= l3)
    //         c = c2;
    // }


    // b.k = 0;
    // b.b = obst->c->y + obst->a;
    // c2 = getIntersection(&b, &b2);
    // if(isDotInside(&c2, obst))
    // {
    //     l2 = getLen(v->c, &c2);
    //     if(l2 < l1 && l2 <= l3)
    //         c = c2;
    // }

    // if(isDotInside(&c, obst))
    // {
    //     if(isDotInside(&c2, obst))
    //     {
    //         l1 = getLen(v->c, &c);
    //         l2 = getLen(v->c, &c2);
    //         if(l1 < l2 && l1 <= l3)
    //             return c;
    //         else if(l2 <= l3)
    //             return c2;
    //         else
    //             return {0, 0};
    //     }
    //     else
    //         return c;
    // }
    // else if(isDotInside(&c2, obst) &&  getLen(v->c, &c2) <= l3)
    //     return c2;
    // else
    //     return {0, 0};
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

    ret.k = ((float)v->dy / (float)v->dx);

    if(v->dx != 0)
        ret.b = v->c->y - v->c->x * ret.k;
    else
        ret.b = v->c->x;

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
    struct coords c = getIntersection(obst, v);
    // std::cout << "obst: " << obst->c->x << ":" << obst->c->y << "; vect: " << v->c->x << ":" << v->c->y;
    if((c.x == 0 && c.y == 0) || c.eq(v->c) || (((v->dx + v->c->x) == c.x) && ((v->dy + v->c->y) == c.y)))
    {
        // std::cout << "have no intersection" << std::endl;
        return false;
    }
    else
    {
        // std::cout << "have intersection" << std::endl;
        return true;
    }
}


void            addTarget(struct graphPoint *start, struct graphPoint *end)
{
    std::cout << "ADD TARGET ";
    if(start == end) 
    {
        std::cout << "ST=EN ";
        return;
    }
    for(size_t i = 0; i < start->numOfTargets; i++)
        if(start->targets[i] == end)
        {
            std::cout << "ALREADY EXISTS ";
            return;
        }
    for(size_t i = 0; i < end->numOfTargets; i++)
        if(end->targets[i] == start)
        {
            std::cout << "ALREADY EXISTS ";
            return;
        }

    start->targets = (struct graphPoint**)realloc(start->targets, (++(start->numOfTargets)) * sizeof(struct graphPoint*));
    start->targets[start->numOfTargets - 1] = end;

    end->targets = (struct graphPoint**)realloc(end->targets, (++(end->numOfTargets)) * sizeof(struct graphPoint*));
    end->targets[end->numOfTargets - 1] = start;
}


COORDS_DATATYPE getWayPrice(struct graphPoint *start, struct graphPoint *end)
{
    return sqrt(pow(start->c.x - end->c.x, 2) + pow(start->c.y - end->c.y, 2));
}

COORDS_DATATYPE getWayPrice(struct coords *start, struct coords *end)
{
    return sqrt(pow(start->x - end->x, 2) + pow(start->y - end->y, 2));
}