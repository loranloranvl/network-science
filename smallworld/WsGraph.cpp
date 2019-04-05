#include <iostream>
#include <iomanip>
#include <math.h>
#include <stdlib.h>
#include <algorithm>
#include <set>
#include "WsGraph.h"
using namespace std;

WsGraph::WsGraph(double ws_p) : p(ws_p) {
    srand(233);
    init_regular();
    for (int i = 0; i < ws_N * ws_K / 2; ++i) {
        reconnect(i % ws_N, (i + i / ws_N + 1) % ws_N);
    }
    desc();
}

void WsGraph::desc() const {
    int width = ceil(log10(ws_N)) + 1;
    for (int i = 0; i < ws_N; ++i) {
        Vertex* pv = this->adj[i];
        cout << "v" << setw(width) << left << i;
        cout << "| " << setw(2) << left << pv->status;

        if (pv->pneighbors->size()) cout << " <-  ";
        for (size_t i = 0; i < pv->pneighbors->size(); ++i) 
            cout << "v" << setw(width) << *next(pv->pneighbors->begin(), i) << " ";

        cout << "\n";
    }

    cout << endl;
}

void WsGraph::init_regular() {
    for (int v = 0; v < ws_N; v++)
        this->adj[v] = new Vertex;
    for (int v = 0; v < ws_N; v++)
        for (int jump = 1; jump < ws_K / 2 + 1; jump++)
            add_edge(v, (v + jump + ws_N) % ws_N);
}

void WsGraph::add_edge(int v_, int _v) {
    if (!this->adj[v_]->pneighbors->insert(_v).second)
        cerr << "[warning] " << _v 
            << " already in adjacency of " << v_ << '\n';
    if (!this->adj[_v]->pneighbors->insert(v_).second)
        cerr << "[warning] " << v_
            << " already in adjacency of " << _v << '\n';
}

void WsGraph::remove_edge(int v_, int _v) {
    if (!this->adj[v_]->pneighbors->erase(_v))
        cerr << "[warning] " << _v 
            << " not in adjacency of " << v_ << '\n';
    if (!this->adj[_v]->pneighbors->erase(v_))
        cerr << "[warning] " << v_ 
            << " not in adjacency of " << _v << '\n';
}

void WsGraph::reconnect(int va, int vb) {
    if ((double)rand() / (double)RAND_MAX > this->p)
        return;
    remove_edge(va, vb);
    set<int>* pe = new set<int>(
        this->adj[va]->pneighbors->begin(),
        this->adj[va]->pneighbors->end()
    );
    pe->insert(va);
    int vc = roulette_select(pe);
    add_edge(va, vc);
}

int WsGraph::roulette_select(set<int>* pexceptions) const {
    set<int>* pe = pexceptions;
    if (pe != nullptr && pe->size() == (unsigned)ws_N) {
        cerr << "[error] all vertices in exception list" << endl;
        return -1;
    }

    vector<int> roulette(ws_N, 0);
    roulette[0] = pe->find(0) == pe->end() ? 1 : 0;
    for (int i = 1; i < ws_N; ++i) {
        if (pe == nullptr || pe->find(i) == pe->end())
            // if i not in `exceptions`
            roulette[i] = roulette[i - 1] + 1;
        else
            roulette[i] = roulette[i - 1];
    }

    int roulette_picker = rand() % roulette[ws_N - 1];

    int lucky_dog = -1;

    // binary search
    int begin = 0;
    int end = ws_N;
    int middle = (begin + end) / 2;

    while (begin < end) {
        if (roulette_picker < roulette[middle])
            end = middle;
        else
            begin = middle + 1;
        middle = (begin + end) / 2;
    }

    lucky_dog = begin;

    // ------ log begin ------

    // cout << "roulette begins";
    // if (pe->size()) cout << "\nexceptions: ";
    // for (int e : *pe)
    //     cout << e << " ";
    // cout << "\n#\tstick\n";

    // for (int i = 0; i < ws_N; ++i)
    //     cout << "v" << i << "\t" 
    //         << roulette[i] << "\n";

    // cout << "roulette picker falls on " << roulette_picker 
    //     << "\nv" << lucky_dog << " is selected"
    //     << endl << endl;

    // ------ log end ------

    if (lucky_dog == -1)
        throw runtime_error("roulette failed");

    return lucky_dog;
}