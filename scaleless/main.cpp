#include "Graph.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{
    for (int j = 5; j < 9; j += 2) {
        Graph my_graph(j, ING_GROWTH, j);
        for (int i = 0; i < 15e3; ++i)
            my_graph.grow();
        char path[256];
        sprintf(path, "dc_equal_m%d.csv", j);
        my_graph.output(path);
    }
    return 0;
}