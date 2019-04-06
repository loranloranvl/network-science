#ifndef WSGRAPH_H
#define WSGRAPH_H

#include <string>
#include <set>
#include <array>
#include <vector>

// start with a ring of `WSN` vertices
// each connected to its `WSK` nearest neighbours by undirected edges
const int ws_N = 1000;
const int ws_K = 10;
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

struct GraphProp{
    double p;
    double Cp;
    double Lp;
    double Tp;
    double r_half;
};


class WsGraph {

public:
    // Graph constructor
    explicit WsGraph(double ws_p);

    // show graph information
    void desc() const;

    // get graph properties
    GraphProp* dump() const;

private:
    // initialize as a regular network
    void init_regular();

    // break the link between `va` and `vb`
    // randomly choose `vc` and link it to `va`
    void reconnect(int va, int vb);

    // calculate the structural properties
    // namely C(p) and L(p)
    void cal_props();

    // breadth first search
    // generates shortest path lengths
    void bfs_lp(int center, 
        std::array<int, ws_N>& path_lengths);

    // breadth first search
    // for virus spreading
    void bfs_virus(int center, double infect_rate);

    // prepare for virus infection
    void init_virus();

    void spread();

    void is_infecting();

    // randomly select a vertex 
    // vetices in `exceptions` will not be considered
    int roulette_select(std::set<int>* pexceptions) const;

    void add_edge(int v_, int _v);

    void remove_edge(int v_, int _v);

    // with probability `p`
    // we reconnect this edge to another vertex
    double p;

    // clustering coefficient C(p)
    double Cp;

    // characteristic path length L(p)
    double Lp;

    // the time required for global infection T(p) with r = 1
    double Tp;

    // probability of infection
    std::array<double, ws_R_SAMPLE_SIZE> r;

    // critical infection rate
    double r_half;

    // maximumly infected corresponding to `r`
    std::array<int, ws_R_SAMPLE_SIZE> max_infect;

    // adjacency list
    std::array<Vertex*, ws_N> adj;

};


#endif