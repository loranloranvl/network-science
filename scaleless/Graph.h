#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <array>

const int GRAPH_SIZE = 2e5;

// ingredient: GROWTH
const int ING_GROWTH = 0b01;

// ingredient: PREFERENTIAL ATTACHMENT
const int ING_PREF = 0b10;

class Graph {

public:
    // Graph constructor
    explicit Graph(int init_nv, int init_i, int init_vne);

    // called every time step
    void grow();

    // write graph data to file for plotting
    void output(std::string file_path) const;

    // show graph information
    void desc() const;

    // getter
    int get_num_vertices() const;

private:
    // add a new vertex with `num_edges` edges
    void grow_normal();

    // randomly select a vertex and connect it
    // with another one
    void grow_stationary();

    // select a vertex 
    // either with higher-degree-preferential randomness
    // or uniformly distributed randomness
    // vetices in `exceptions` will not be considered
    int roulette_select(const std::vector<int> & exceptions) const;

    // update adjacent list
    void add_edge(int v1, int v2);

    // adjacent list
    std::array<std::vector<int>*, GRAPH_SIZE> adj_list;

    // quantity of vertices populated in the graph
    int num_vertices;

    // GROWTH, PREFERENTIAL ATTACHMENT or both
    int ingredients;

    // `vnew_num_edges` edges will be added
    // if a new vertex is added
    int vnew_num_edges;

};

#endif