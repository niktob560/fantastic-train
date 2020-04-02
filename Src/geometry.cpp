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
        struct coords nc;
        for(size_t i = 0; i < numOfObstacles; i++)
        {
            nc = *c;
            nc = nc.rotate(obstacles[i].c, obstacles[i].rot);
            if(	(nc.x - CONST > obstacles[i].c->x - (obstacles[i].a / 2))
            &&  (nc.x + CONST < obstacles[i].c->x + (obstacles[i].a / 2))
            &&  (nc.y - CONST > obstacles[i].c->y - (obstacles[i].b / 2))
            &&  (nc.y + CONST < obstacles[i].c->y + (obstacles[i].b / 2)))
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
        // struct vect nv = *v;
        struct coords c0, c1;
        // struct coords nz = *nv.c, ne = *nv.c, c0, c1;
        // ne.x += v->dx;
        // ne.y += v->dy;
        // nz = nz.rotate(obst->c, obst->rot);
        // ne = ne.rotate(obst->c, obst->rot);
        // nv = createVect(&nz, &ne);

        struct obstacle nobst = *obst;
        nobst.a -= 0.1;
        nobst.b -= 0.1;

        c0 = *getCoordsOfCorner(&nobst, Corner::LEFT_TOP);
        c1 = *getCoordsOfCorner(&nobst, Corner::LEFT_BOTTOM);
        struct vect v0 = createVect(&c0, &c1);
        if(hasIntersection(&v0, v))
            return true;


        c0 = *getCoordsOfCorner(&nobst, Corner::RIGHT_TOP);
        c1 = *getCoordsOfCorner(&nobst, Corner::RIGHT_BOTTOM);
        v0 = createVect(&c0, &c1);
        if(hasIntersection(&v0, v))
            return true;

        c0 = *getCoordsOfCorner(&nobst, Corner::RIGHT_BOTTOM);
        c1 = *getCoordsOfCorner(&nobst, Corner::LEFT_BOTTOM);
        v0 = createVect(&c0, &c1);
        if(hasIntersection(&v0, v))
            return true;


        c0 = *getCoordsOfCorner(&nobst, Corner::RIGHT_TOP);
        c1 = *getCoordsOfCorner(&nobst, Corner::LEFT_TOP);
        v0 = createVect(&c0, &c1);
        if(hasIntersection(&v0, v))
            return true;
        

        struct coords c = getIntersection(&nobst, v); 
        if(!c.eq({0, 0}))
            return true;
        else
        // if((c.eq({0, 0})) || c.eq(nv.c) || (((nv.dx + nv.c->x) == c.x) && ((nv.dy + nv.c->y) == c.y)))
        //     return false;
        // else
            return false;
    }


    bool            hasIntersection(const struct baseline *b1,  const struct baseline *b2)
    {
        return b1->k != b2->k;
    }

    struct coords   getIntersection(const struct baseline *b1, const struct baseline *b2)
    {
        struct coords ret;
        if(b1->k == b2->k)
        {
            if(b1->b != b2->b)
                ret = {0, 0};
            else
                ret = {1, b1->b + b1->k};
        }
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

    bool hasIntersection(const struct vect *v1, const struct vect *v2)
    {
        struct vect v;
        struct baseline b1 = vectToBaseline(v1), 
                        b2 = vectToBaseline(v2);
        COORDS_DATATYPE l1, l2;
        struct coords   c = getIntersection(&b1, &b2), 
                        c0;
        if(c.eq({0, 0}))
            return false;

        v = *v1;                //рассматриваем первый вектор
        l1 = getLen(&c, v.c);   //от начала до пересечения
        l2 = getLen(&v);        //длина вектора
        if(l1 >= l2)             //если от начала до пересечения дальше чем длина вектора
            return false;       //то пересечения нет
        
        c0 = v.getLastCoords(); //конец вектора
        l1 = getLen(&c, &c0);   //от конца до пересечения
        if(l1 >= l2)             //если от конца до пересечения дальше чем длина вектора
            return false;       //то пересечения нет
        
        v = *v2;                //рассматриваем второй вектор
        l1 = getLen(&c, v.c);   //от начала до пересечения
        l2 = getLen(&v);        //длина вектора
        if(l1 >= l2)             //если от начала до пересечения дальше чем длина вектора
            return false;       //то пересечения нет
        
        c0 = v.getLastCoords(); //конец вектора
        l1 = getLen(&c, &c0);   //от конца до пересечения
        if(l1 >= l2)             //если от конца до пересечения дальше чем длина вектора
            return false;       //то пересечения нет
        
        return true;
    }

    struct coords getIntersection(const struct vect *v1, const struct vect *v2)
    {
        if(hasIntersection(v1, v2))
        {
            struct baseline b1 = vectToBaseline(v1), 
                            b2 = vectToBaseline(v2);
            return getIntersection(&b1, &b2);
        }
        else
            return {0, 0};
    }

    struct coords   getIntersection(const struct obstacle *obst, const struct vect *v)
    {
        // if(hasIntersection(obst, v))
        // {
        //     struct coords c0, c1, int1, int0;
        //     COORDS_DATATYPE l0, l1;
        //     c0 = *getCoordsOfCorner(obst, Corner::LEFT_BOTTOM);
        //     c1 = *getCoordsOfCorner(obst, Corner::LEFT_TOP);
        //     struct vect v0 = createVect(&c0, &c1),
        //                 v1;
        //     c0 = *getCoordsOfCorner(obst, Corner::RIGHT_BOTTOM);
        //     c1 = *getCoordsOfCorner(obst, Corner::RIGHT_TOP);
        //     v1 = createVect(&c0, &c1);
        //     int0 = getIntersection(v, &v0);
        //     int1 = getIntersection(v, &v1);
        //     if(int0.eq({0, 0}))
        //         int0 = int1;
        //     else if(!int1.eq({0, 0}))
        //     {
        //         l0 = getLen(v->c, &int0);
        //         l1 = getLen(v->c, &int1);
        //         if(l1 < l0)
        //             int0 = int1;
        //     }

        //     c0 = *getCoordsOfCorner(obst, Corner::LEFT_BOTTOM);
        //     c1 = *getCoordsOfCorner(obst, Corner::RIGHT_BOTTOM);
        //     v1 = createVect(&c0, &c1);
        //     int1 = getIntersection(v, &v1);
        //     if(int0.eq({0, 0}))
        //         int0 = int1;
        //     else if(!int1.eq({0, 0}))
        //     {
        //         l0 = getLen(v->c, &int0);
        //         l1 = getLen(v->c, &int1);
        //         if(l1 < l0)
        //             int0 = int1;
        //     }

        //     c0 = *getCoordsOfCorner(obst, Corner::LEFT_TOP);
        //     c1 = *getCoordsOfCorner(obst, Corner::RIGHT_TOP);
        //     v1 = createVect(&c0, &c1);
        //     int1 = getIntersection(v, &v1);
        //     if(int0.eq({0, 0}))
        //         int0 = int1;
        //     else if(!int1.eq({0, 0}))
        //     {
        //         l0 = getLen(v->c, &int0);
        //         l1 = getLen(v->c, &int1);
        //         if(l1 < l0)
        //             int0 = int1;
        //     }

        //     return int0;
            
        // }
        // else
        //     return {0, 0};

        struct vect nv = *v;
        struct coords nz = *nv.c, ne = *nv.c;
        ne.x += v->dx;
        ne.y += v->dy;
        nz = nz.rotate(obst->c, obst->rot);
        ne = ne.rotate(obst->c, obst->rot);
        nv = createVect(&nz, &ne);

        struct coords c1, c2, tgt, inter1, inter2;
        tgt = {(nv.dx + nv.c->x), (nv.dy + nv.c->y)};



        
        struct baseline b1,
                        b2;
        COORDS_DATATYPE l1 = 0, l2 = 0, lv = 0;

        lv = getLen(&nv);

        b1 = vectToBaseline(&nv);

        b2.k = INF;         //vertical line
        b2.b = obst->c->x;  //that located in left side

        inter1 = getIntersection(&b1, &b2); //intersection between left side and vector line
        if(isDotInside(&inter1, obst))      //check if dot is inside or on edge of obstacle
        {
            l1 = getLen(nv.c, &inter1);     //get len from beginning of vector to intersection with obstacle
            if(!(isCoDirectional(v, &inter1) && l1 <= lv))  //if intersection is not located on vector
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
            l2 = getLen(nv.c, &inter2);
            if(!(isCoDirectional(&nv, &inter2) && l2 <= lv))
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
            l2 = getLen(nv.c, &inter2);
            if(!(isCoDirectional(&nv, &inter2) && l2 <= lv))
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
            l2 = getLen(nv.c, &inter2);
            if(!(isCoDirectional(&nv, &inter2) && l2 <= lv))
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
        struct coords ndot = *dot;
        ndot = ndot.rotate(obstacle->c, obstacle->rot);
        return      (ndot.x >= obstacle->c->x - (obstacle->a / 2))
                &&  (ndot.x <= obstacle->c->x + (obstacle->a / 2))
                &&  (ndot.y >= obstacle->c->y - (obstacle->b / 2))
                &&  (ndot.y <= obstacle->c->y + (obstacle->b / 2));
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
        *ret = ret->rotate(obst->c, obst->rot);
        return ret;
    }

    struct coords  _getCoordsOfCorner(const struct obstacle *obst, const Corner corner)
    {
        struct coords ret;
        switch (corner) {
            case Corner::LEFT_BOTTOM:
            {
                ret.x = obst->c->x - CONST - (obst->a / 2);
                ret.y = obst->c->y - CONST - (obst->b / 2);
                break;
            }
            case Corner::LEFT_TOP:
            {
                ret.x = obst->c->x - CONST - (obst->a / 2);
                ret.y = obst->c->y + CONST + (obst->b / 2);
                break;
            }
            case Corner::RIGHT_BOTTOM:
            {
                ret.x = obst->c->x + CONST + (obst->a / 2);
                ret.y = obst->c->y - CONST - (obst->b / 2);
                break;
            }
            case Corner::RIGHT_TOP:
            {
                ret.x = obst->c->x + CONST + (obst->a / 2);
                ret.y = obst->c->y + CONST + (obst->b / 2);
                break;
            }
        }
        ret = ret.rotate(obst->c, obst->rot);
        return ret;
    }


    struct graphbases::graphPoint* getPoint(const struct obstacle *obst, const Corner corner)
    {
        return obst->corners[corner];
    }

}
