#include "geometry.hpp"
#include <stdint.h>
#include "bases.hpp"

namespace geometry
{
	using namespace bases;

    struct vect createVect(const struct coords *c, const COORDS_DATATYPE angle)
    {
        struct vect ret;
        ret.c  = (struct coords*)malloc(sizeof(struct coords));
        ret.c->x = c->x;
        ret.c->y = c->y;
        ret.dx = FIELD_MAX_LEN * 10.0 / tan(angle / 180.0 * PI);
        ret.dy = ret.dx * tan(angle / 180.0 * PI);
        ret.dx = abs(ret.dx);
        ret.dy = abs(ret.dy);
        if(angle > 180 || angle < 0)
            ret.dy *= -1;

        if(!((angle >= -90 && angle <= 90) || (angle >= 190 + 90 && angle <= 360 + 90)))
            ret.dx *= -1;
        return ret;
    }


    struct vect createVect(const struct coords *start, const struct coords *end)
    {
        struct vect ret;
        ret.c = (struct coords*)malloc(sizeof(struct coords));
        ret.c->x = start->x;
        ret.c->y = start->y;
        ret.dx = end->x - start->x;
        ret.dy = end->y - start->y;
        return ret;
    }

    struct vect createVect(const struct obstacle *obst, const Corner corner, const COORDS_DATATYPE angle)
    {
        struct vect ret;
        struct coords *c = getCoordsOfCorner(obst, corner);
        ret = createVect(c, angle);
        free(c);
        return ret;
    }

    bool hasIntersections(const struct vect *v)
    {
        for(size_t i = 0; i < numOfObstacles; i++)
        {
            if(hasIntersection(&obstacles[i], v))
                return true;
        }
        return false;
    }

    #define CONST 0.4

    bool isDotInside(const struct coords *c)
    {
        for(size_t i = 0; i < numOfObstacles; i++)
        {
            if(	(c->x - CONST > obstacles[i].c->x - (obstacles[i].a / 2))
            &&  (c->x + CONST < obstacles[i].c->x + (obstacles[i].a / 2))
            &&  (c->y - CONST > obstacles[i].c->y - (obstacles[i].b / 2))
            &&  (c->y + CONST < obstacles[i].c->y + (obstacles[i].b / 2)))
                return true;
        }
        return false;
    }

    #undef CONST



    struct coords getCoordsOfPoint(struct graphbases::graphPoint *p)
    {
        return (p->c);
    }

    bool            hasIntersection(const struct obstacle *obst, const struct vect *v)
    {
        struct coords c = getIntersection(obst, v);
        if((c.x == 0 && c.y == 0) || c.eq(v->c) || (((v->dx + v->c->x) == c.x) && ((v->dy + v->c->y) == c.y)))
            return false;
        else
            return true;
    }


    bool            hasIntersection(const struct baseline *b1,  const struct baseline *b2)
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

        
        b2.b += obst->b;

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
    }




    bool            isDotInside(const struct coords *dot, const struct obstacle *obstacle)
    {
        return      (dot->x >= obstacle->c->x - (obstacle->a / 2))
                &&  (dot->x <= obstacle->c->x + (obstacle->a / 2))
                &&  (dot->y >= obstacle->c->y - (obstacle->b / 2))
                &&  (dot->y <= obstacle->c->y + (obstacle->b / 2));
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

    #define CONST 0.4

    struct coords*  getCoordsOfCorner(const struct obstacle *obst, const Corner corner)
    {
        struct coords *ret = (struct coords*)malloc(sizeof(struct coords));
        switch (corner) {
            case Corner::LEFT_BOTTOM:
            {
                ret->x = obst->c->x - CONST - (obst->a / 2);
                ret->y = obst->c->y - CONST - (obst->b / 2);
                break;
            }
            case Corner::LEFT_TOP:
            {
                ret->x = obst->c->x - CONST - (obst->a / 2);
                ret->y = obst->c->y + CONST + (obst->b / 2);
                break;
            }
            case Corner::RIGHT_BOTTOM:
            {
                ret->x = obst->c->x + CONST + (obst->a / 2);
                ret->y = obst->c->y - CONST - (obst->b / 2);
                break;
            }
            case Corner::RIGHT_TOP:
            {
                ret->x = obst->c->x + CONST + (obst->a / 2);
                ret->y = obst->c->y + CONST + (obst->b / 2);
                break;
            }
        }
        return ret;
    }


    struct graphbases::graphPoint* getPoint(const struct obstacle *obst, const Corner corner)
    {
        return obst->corners[corner];
    }


}
