#include "pathfinder.hpp"
#include "bases.hpp"
#include "math.hpp"

namespace pathfinder
{

    using namespace bases;


    bool hasUncalculatedPoints()
    {
        for(size_t i = 0; i < graphSize; i++)
            if(!graph[i]->calculated)
                return true;
        return false;
    }
    
    struct graphPoint* getMinTarget()
    {
        COORDS_DATATYPE w = INF;
        struct graphPoint * pp = 0;
        for(size_t i = 0; i < graphSize; i++)
        {
            if(!graph[i]->calculated && graph[i]->weight <= w)
            {
                pp = graph[i];
                w = pp->weight;
            }
        }
        return pp;
    }

    void calculateWay(size_t index) 
    {
        if(graph[index] == NULL || graph[index]->calculated)    //Если точки не существует либо она уже обработана
            return;                                             //                  завершить

        graph[index]->calculated = true;                        //Отметить как обработанную
        
        if(graph[index]->numOfTargets == 0)                     //Если целей нет то завершить
            return;

        struct graphPoint * watch;
        COORDS_DATATYPE w;

        for(size_t i = 0; i < graph[index]->numOfTargets; i++)  //Итерируемся по всем целям точки
        {
            watch = graph[index]->targets[i];                   //Текущая цель
            if(!watch->calculated)
            {
                w = getWayPrice(graph[index], watch);               //Цена пути от текущей точки до цели
                if(graph[index]->weight + w < watch->weight)        //Если полученная цена меньше чем имеющаяся
                {
                    watch->weight = w + graph[index]->weight;       //Установим новую цену
                    ways[watch->i] = index;                         //Запишем об этом в вектор путей
                }
            }
        }

        while (hasUncalculatedPoints())             //Если есть необработанные цели
        {
            watch = getMinTarget();                 //Берём с наименьшей ценой
            calculateWay(watch->i);                             //Вычисляем её рекурсивно
        }
    }


    COORDS_DATATYPE getWayPrice(struct graphPoint *start, struct graphPoint *end)
    {
        return sqrt(pow(start->c.x - end->c.x, 2) + pow(start->c.y - end->c.y, 2));
    }

    COORDS_DATATYPE getWayPrice(struct coords *start, struct coords *end)
    {
        return sqrt(pow(start->x - end->x, 2) + pow(start->y - end->y, 2));
    }
}