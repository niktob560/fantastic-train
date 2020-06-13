#include <graphbases.hpp>
#include <bases.hpp>

namespace fantastictrain {
    
    void calculate();

    void flush();

    void addObstacle(bases::obstacle o);

    void loadGraph(graphbases::graphPoint** graph);

    graphbases::graphPoint** getGraph();

    graphbases::graphPoint** getWay();

    void setTarget(size_t index);

    void addHome(size_t index);

    bases::obstacle* getObstacle(size_t index);

    bases::obstacle** getObstacles();

}
