#include "Graph.h"

int main(int argc, char const *argv[])
{
    Graph my_graph(10, ING_GROWTH | ING_PREF, 3);
    // my_graph.desc();
    for (int i = 0; i < 5; ++i)
        my_graph.grow();
    // my_graph.desc();
    return 0;
}