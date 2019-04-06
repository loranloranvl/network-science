#include "WsGraph.h"
#include <cmath>
#include <fstream>
#include <iostream>
using namespace std;

void model();
void test();

int main() {
    model();
    
    return 0;
}

void model() {
    srand(233);
    double p_begin = 0;
    double p_distance = -4;
    double p_sticks = 20;
    double p_step = p_distance / p_sticks;
    ofstream writer("data/graph-properties.csv", ios::out);
    writer << "p,Cp,Lp,Tp,rhalf\n";
    for (int i = 0; i < p_sticks; ++i) {
        WsGraph my_graph(pow(10, (p_begin + p_step) * i));
        GraphProp* pgp = my_graph.dump();
        cout << i << endl;
        writer << pgp->p << ',' << pgp->Cp << ',' 
            << pgp->Lp << ',' << pgp->Tp << ',' 
            << pgp->r_half << '\n';
    }
    WsGraph my_graph(0);
    GraphProp* pgp = my_graph.dump();
    writer << pgp->p << ',' << pgp->Cp << ',' 
        << pgp->Lp << ',' << pgp->Tp << ',' 
        << pgp->r_half << '\n';
}