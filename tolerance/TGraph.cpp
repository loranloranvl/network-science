#include "TGraph.h"
using namespace std; 

TGraph::TGraph(int nodes_num) {
    this->num_vertices = nodes_num; 
}

void TGraph::exp_gen() {
    for (int i = 0; i < this->adj_list.size(); ++i) {
        for (int j = 0; j <= i; j++) {
            if (rand() % 1000 <= 1) {
                Graph::add_edge(i, j);
            }
        }
    }
}

void TGraph::sf_gen() {
    for (int i = 0;  i  <  2e5+1;  ++i) {
        Graph::grow(); 
    }
}

void TGraph::diameter() {
    for (int i = 0; i < this->adj_list.size(); ++i){
        for (int j = 0; j < this->adj_list.size(); ++j){
            this->adj_list[i][j] = -1; 
        }
        this->adj_list[i][i] = 0; 
    }

    //edges count
    int m = 0;
    for (int i = 0; i < this->adj_list.size(); ++i) {
        m += this->adj_list[i].size();
    }

    while(m--){
        //nodes - let the indexation begin from 1
        int a, b; 

        //edge weight
        int c; 

        scanf("%d %d %d", &a, &b, &c); 
        this->adj_list[a][b] = c; 
    }

    //Floyd-Warshall
    for (int k = 1; k <= this->num_vertices; ++k){
        for (int i = 1; i <= this->num_vertices; ++i){
            if (this->adj_list[i][k] != -1){
                for (int j = 1; j <= this->num_vertices; ++j){
                    if (this->adj_list[k][j] != -1 && (this->adj_list[i][j] = -1 || this->adj_list[i][k]+this->adj_list[k][j] < this->adj_list[i][j])){
                        this->adj_list[i][j] = this->adj_list[i][k]+this->adj_list[k][j]; 
                    }
                }
            }
        }
    }

    this->diameter = -1; 

    //look for the most distant pair
    for (int i = 1; i <= this->num_vertices; ++i){
        for (int j = 1; j <= this->num_vertices; ++j){
            if (this->diameter < this->adj_list[i][j]){
                this->diameter = this->adj_list[i][j]; 
                printf("%d %d\n", i, j); 
            }
        }
    }

    return 0; 
}

void TGraph::failure_sim() {
    for (int i = 0; i < 100; ++i) {
        int t1 = rand() % this->adj_list.size();
        remove_node(t1);
    }
}

void TGraph::attack_sim() {
    for (int i = 0; i < 100; ++i) {
        int t1 = 0;
        for (int j = 0; j < this->adj_list.size(); j++) {
            if (this->adj_list[j].size() > this->adj_list[t1].size()) {
                t1 = j;
            }
        }
        remove_node(t1);
    }
}

double TGraph::get_diameter() const {
    return this->diameter; 
}