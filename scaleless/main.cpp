#include "Graph.h"
#include <stdio.h>
#include <iostream>
using namespace std;

int main()
{

    // GROWTH and PREFERENTIAL ATTACHMENT
    /*
    Graph my_graph(5, ING_GROWTH | ING_PREF, 5);
    for (int i = 0; i < 2e5+1; ++i) {
        my_graph.grow();
        if (i % 1000 == 0)
            cout << "v" << i << " is added" << endl;
        if (i + 1 == 1e5 || i + 1 == 1.5e5 || i + 1 == 2e5) {
            char path[256];
            sprintf(path, "data/dc_t%dw.csv", i / 10000 + 1);
            my_graph.output(path);
        }
    }
    */
   
   // GROWTH
   /*
   for (int m = 1; m < 9; m += 2) {
        Graph my_graph(5, ING_GROWTH | ING_PREF, 0);
        for (int i = 0; i < 2e5; ++i)
            my_graph.grow();
        char path[256];
        sprintf(path, "data/dc_growth_m%d.csv", m);
        my_graph.output(path);
   }
   */

   return 0;
}