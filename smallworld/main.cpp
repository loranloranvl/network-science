#include "WsGraph.h"
#include <cmath>
#include <fstream>
#include <iostream>
using namespace std;

void model();
void test();

int main() {
    test();
    // model();
    
    return 0;
}

void model() {
    srand(2333);
    double p_begin = 0;
    double p_distance = -4;
    double p_sticks = 16;
    double p_step = p_distance / p_sticks;
    ofstream writer("data/graph-properties.csv", ios::out);
    writer << "p,Cp,Lp\n";
    for (int i = 0; i < p_sticks; ++i) {
        WsGraph my_graph(pow(10, (p_begin + p_step) * i));
        // GraphProp* pgp = my_graph.dump();
        cout << i << endl;
        // writer << pgp->p << ',' << pgp->Cp << ',' 
            // << pgp->Lp << '\n';
    }
    WsGraph my_graph(0);
    // GraphProp* pgp = my_graph.dump();
    // writer << pgp->p << ',' << pgp->Cp << ',' 
        // << pgp->Lp << '\n';
}

void test() {
    srand(233);
    WsGraph wg(0.5);
    cout << wg.dump()->Lp << endl;
}