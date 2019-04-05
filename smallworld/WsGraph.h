#ifndef WSGRAPH_H
#define WSGRAPH_H

#include <string>
#include <set>
#include <array>
#include <vector>

// start with a ring of `WSN` vertices
// each connected to its `WSK` nearest neighbours by undirected edges
const int ws_N = 20;
const int ws_K = 4;
const int ws_R_SAMPLE_SIZE = 100;


struct Vertex{
    std::set<int>* pneighbors;

    // for virus infection
    // 1: normal; 0: infected; -1: dead or immunized
    int status;

    Vertex() : 
        pneighbors(new std::set<int>),
        status(1) {}

};


class WsGraph {

public:
    // Graph constructor
    explicit WsGraph(double ws_p);

    // show graph information
    void desc() const;

    // write necessary data to file
    void dump() const;

private:
    // initialize as a regular network
    void init_regular();

    // break the link between `va` and `vb`
    // randomly choose `vc` and link it to `va`
    void reconnect(int va, int vb);

    // calculate the structural properties
    // namely C(p) and L(p)
    void cal_props();

    // prepare for virus infection
    void init_virus();

    // randomly select a vertex 
    // vetices in `exceptions` will not be considered
    int roulette_select(std::set<int>* pexceptions) const;

    void spread();

    void add_edge(int v_, int _v);

    void remove_edge(int v_, int _v);

    // with probability `p`
    // we reconnect this edge to another vertex
    double p;

    // time `t` for virus spreading
    int t;

    // clustering coefficient C(p)
    int Cp;

    // characteristic path length L(p)
    int Lp;

    // the time required for global infection T(p) with r = 1
    int Tp;

    // probability of infection
    int r[ws_R_SAMPLE_SIZE];

    // maximumly infected corresponding to `r`
    int max_infect[ws_R_SAMPLE_SIZE];

    // adjacency list
    std::array<Vertex*, ws_N> adj;

};


#endif