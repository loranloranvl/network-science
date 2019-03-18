#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <math.h>
#include "Graph.h"
using namespace std;

Graph::Graph(int init_nv, int init_i, int init_vne)
        : num_vertices(init_nv),
          ingredients(init_i),
          vnew_num_edges(init_vne)
{
    assert(init_nv <= GRAPH_SIZE && init_vne <= init_nv);

    for (int i = 0; i < init_nv; ++i) 
        this->adj_list[i] = new vector<int>;

    // ------ log begin ------
    
    cout << "Graph object initialized with "
        << init_nv << " vertices\n"
        << "every new vertex with " << init_vne
        << " edges\ningredients: ";
    if (init_i & ING_GROWTH == ING_GROWTH)
        cout << "GROWTH; ";
    if (init_i & ING_PREF == ING_PREF)
        cout << "PREFERENTIAL ATTACHMENT; ";
    cout << endl << endl;

    // ------ log end ------
    
    srand(233);
}

void Graph::grow() {
    if (this->ingredients & ING_GROWTH == ING_GROWTH) {
        grow_normal();
    } else {
        grow_stationary();
    }
}

void Graph::output(string file_path) const {
    // pass
}

int Graph::get_num_vertices() const {
    return this->num_vertices;
}

void Graph::desc() const {
    cout << get_num_vertices() << " vertices here\n";
    int width = ceil(log10(this->num_vertices)) + 1;
    for (int i = 0; i < this->num_vertices; ++i) {
        cout << "v" << setw(width) << left << i;
        vector<int>* pneighbors = this->adj_list[i];
        if (pneighbors->size()) cout << "<-  ";
        for (size_t i = 0; i < pneighbors->size(); ++i)
            cout << "v" << setw(width) << (*pneighbors)[i] << " ";
        cout << "\n";
    }
    cout << endl;
}

void Graph::grow_normal() {
    assert(this->num_vertices + 1 <= GRAPH_SIZE);

    int vnew = num_vertices;
    this->adj_list[vnew] = new vector<int>;

    vector<int> exceptions;
    for (int i = 0; i < this->vnew_num_edges; ++i) {
        int vtarget = roulette_select(exceptions);
        exceptions.push_back(vtarget);
        add_edge(vtarget, vnew);
    }
    
    num_vertices++;

    // ------ log begin ------
    
    cout << "v" << vnew << " is added" << endl;
    desc();

    // ------ log end ------
}

void Graph::grow_stationary() {
    int v1 = rand() % this->num_vertices;

    int vnew = 0;

    // todo: whatever
    
    add_edge(v1, vnew);
}

int Graph::roulette_select(const vector<int> & exceptions) const {
    const vector<int> & es = exceptions;
    int with_pref = this->ingredients & ING_PREF == ING_PREF;

    vector<int> roulette(this->num_vertices, 0);
    roulette[0] = (with_pref ? adj_list[0]->size() : 0) + 1;
    for (int i = 1; i < this->num_vertices; ++i) {
        if (find(es.begin(), es.end(), i) == es.end())
            // if i not in `exceptions`
            roulette[i] = roulette[i - 1] 
                + (with_pref ? adj_list[i]->size() : 0) + 1;
        else
            roulette[i] = roulette[i - 1];
    }

    int total_sum = roulette[this->num_vertices - 1];
    if (total_sum == 1) {
        cerr << "[error] all vertices in exception list" << endl;
        return -1;
    }

    int roulette_picker = rand() % total_sum;

    int lucky_dog = -1;
    for (int i = 0; i < this->num_vertices; ++i)
        if (roulette_picker < (roulette[i])) {
            lucky_dog = i;
            break;
        }

    // ------ log begin ------

    // cout << "roulette begins";
    // if (es.size()) cout << "\nexceptions: ";
    // for (int e : es)
    //     cout << e << " ";
    // cout << "\n#\tdegree\tstick\n";

    // for (int i = 0; i < this->num_vertices; ++i)
    //     cout << "v" << i << "\t" 
    //         << this->adj_list[i]->size() << "\t"
    //         << roulette[i] << "\n";

    // cout << "roulette picker falls on " << roulette_picker 
    //     << "\nv" << lucky_dog << " is selected preferentially"
    //     << endl << endl;

    // ------ log end ------

    if (lucky_dog == -1)
        throw runtime_error("roulette failed");

    return lucky_dog;
}

void Graph::add_edge(int v1, int v2) {
    this->adj_list[v1]->push_back(v2);
    this->adj_list[v2]->push_back(v1);
}