// tolerance attack testing graph module

#ifndef TGRAPH_H
#define TGRAPH_H

#include <array>
#include <vector>

class TGraph : public Graph {
public:
    // TGraph constructor
    explicit TGraph(int nodes_num);

    // exponential graph generator
    void exp_gen();

    // scale-free graph generator
    void sf_gen();

    // failure simulation
    void failure_sim();

    // attack simulation
    void attack_sim();

    // calculate graph diameter
    void diameter();

    // diameter getter
    double get_diameter() const;
private:
    int num_vertices;
    double diameter;
    std::array<std::vector<int>*, GRAPH_SIZE> adj_list;
}

#endif