#include "pathfinder.hpp"

namespace pathfinder
{

    using namespace bases;


    bool hasUncalculatedPoints()
    {
        for(size_t i = 0; i < graphSize; i++)
            if(!graph[i].calculated)
                return true;
        return false;
    }
    
    struct graphPoint* getMinTarget()
    {
        uint16_t w = INF;
        struct graphPoint * pp = &graph[0];
        for(size_t i = 0; i < graphSize; i++)
        {
            if(!graph[i].calculated && graph[i].weight <= w)
            {
                pp = &graph[i];
                w = pp->weight;
            }
        }
        return pp;
    }

    void calculateWay(uint8_t index)
    {
        if(index >= graphSize || graph[index].calculated)    //Если точки не существует либо она уже обработана
            return;                                             //                  завершить

        graph[index].calculated = true;                        //Отметить как обработанную
        
        if(graph[index].numOfTargets == 0)                     //Если целей нет то завершить
            return;

        struct graphPoint * watch;
        uint16_t w;

        for(size_t i = 0; i < graph[index].numOfTargets; i++)  //Итерируемся по всем целям точки
        {
            watch = graph[index].targets[i];                   //Текущая цель
            if(!watch->calculated)
            {
                w = getWayPrice(&graph[index], watch);               //Цена пути от текущей точки до цели
                if(graph[index].weight + w < watch->weight)        //Если полученная цена меньше чем имеющаяся
                {
                    watch->weight = static_cast<uint16_t>(w + graph[index].weight);       //Установим новую цену
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


    uint16_t getWayPrice(struct graphPoint *start, struct graphPoint *end)
    {
        const struct coords c1 = geometry::getCoordsOfPoint(start), 
                            c2 = geometry::getCoordsOfPoint(end);
        return static_cast<uint16_t>(sqrt(pow(c1.x - c2.x, 2) + pow(c1.y - c2.y, 2)));
    }

    uint16_t getWayPrice(struct coords *start, struct coords *end)
    {
        return static_cast<uint16_t>(sqrt(pow(start->x - end->x, 2) + pow(start->y - end->y, 2)));
    }
}