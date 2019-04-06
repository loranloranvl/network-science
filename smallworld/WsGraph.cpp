#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <algorithm>
#include <set>
#include <cmath>
#include <numeric>
#include <cassert>
#include <queue>
#include <fstream>
#include "WsGraph.h"
using namespace std;

WsGraph::WsGraph(double ws_p) 
    : p(ws_p), Cp(0), Lp(0), Tp(0)
{
    init_regular();
    for (int i = 0; i < ws_N * ws_K / 2; ++i) {
        reconnect(i % ws_N, (i + i / ws_N + 1) % ws_N);
    }
    cal_props();
    init_virus();
    spread();
}

void WsGraph::desc() const {
    int width = ceil(log10(ws_N)) + 1;
    for (int i = 0; i < ws_N; ++i) {
        Vertex* pv = this->adj[i];
        cout << "v" << setw(width) << left << i;
        cout << "| " << setw(2) << left << pv->status;

        if (pv->pneighbors->size()) cout << " <-  ";
        for (size_t j = 0; j < pv->pneighbors->size(); ++j) 
            cout << "v" << setw(width) << *next(pv->pneighbors->begin(), j) << " ";

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
    if (1.0 * rand() / RAND_MAX > this->p)
        return;
    remove_edge(va, vb);

    // exceptions
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

    if (lucky_dog == -1)
        throw runtime_error("roulette failed");

    return lucky_dog;
}

void WsGraph::cal_props() {
    /* --- characteristic path length L(p) --- */

    // shortest path length
    array<int, ws_N> spl;

    for (int v = 0; v < ws_N; ++v) {
        for (int i = 0; i < ws_N; ++i)
            spl[i] = -1;
        spl[v] = 0;

        bfs_lp(v, spl);

        if (spl.end() != find(
                spl.begin(),
                spl.end(),
                -1
            )) {
            throw runtime_error("[error] bfs_lp failed");
        }

        double avg = accumulate(
            spl.begin(), 
            spl.end(), 
            0.0
        ) / (ws_N - 1); // exclude 0

        this->Lp = (v * this->Lp + avg) / (v + 1);
        if (v == 0) this->Tp = avg;
    }

    /* --- clustering coefficient C(p) --- */
    for (int v = 0; v < ws_N; ++v) {
        int edge_num_neighbors = 0;
        set<int>* pn = this->adj[v]->pneighbors;

        for (auto it1 = pn->cbegin(); it1 != pn->cend(); ++it1) {
            for (auto it2 = next(it1, 1); it2 != pn->cend(); ++it2) {

                set<int>* pn1 = this->adj[*it1]->pneighbors;
                if (pn1->find(*it2) != pn1->end())
                    ++edge_num_neighbors;

            }
        }

        int edge_num_max = pn->size() * (pn->size() - 1) / 2;
        double fraction = 1.0 * edge_num_neighbors / edge_num_max;
        this->Cp = (v * this->Cp + fraction) / (v + 1);
    }
}

void WsGraph::bfs_lp(int center, 
        array<int, ws_N>& path_lengths) {
    array<int, ws_N>& spl = path_lengths;
    queue<int> queue_vertices;
    do {
        for (
            auto it = this->adj[center]->pneighbors->cbegin();
            it != this->adj[center]->pneighbors->cend();
            ++it
        ) {
            if (spl[*it] == -1) {
                spl[*it] = spl[center] + 1;
                queue_vertices.push(*it);
            }
        }
        center = queue_vertices.front();
        queue_vertices.pop();
    } while (!queue_vertices.empty());
}

void WsGraph::bfs_virus(int center, double infect_rate) {
    queue<int> queue_vertices;
    // cout << "bfs init\n*it\tcenter\tr\tq_size\n";
    // cout << "null\t" << center << '\t' << infect_rate
        // << '\t' << queue_vertices.size() << '\n';
    while (true) {
        for (
            auto it = this->adj[center]->pneighbors->cbegin();
            it != this->adj[center]->pneighbors->cend();
            ++it
        ) {
            if (this->adj[*it]->status == 1
                    && 1.0 * rand() / RAND_MAX < infect_rate) {
                this->adj[*it]->status = 0;
                queue_vertices.push(*it);
            }
            // cout << *it << '\t' << center << '\t' << infect_rate
                // << '\t' << queue_vertices.size() << '\n';
        }
        this->adj[center]->status = -1;
        if (queue_vertices.empty()) break;
        center = queue_vertices.front();
        queue_vertices.pop();
    }
}

GraphProp* WsGraph::dump() const {
    GraphProp* pgp = new GraphProp;
    pgp->p = this->p;
    pgp->Cp = this->Cp;
    pgp->Lp = this->Lp;
    pgp->Tp = this->Tp;
    pgp->r_half = this->r_half;
    return pgp;
}

void WsGraph::init_virus() {
    double r_begin = 0.1;
    double r_end = 0.4;
    double r_step = (r_end - r_begin) / ws_R_SAMPLE_SIZE;
    for (int i = 0; i < ws_R_SAMPLE_SIZE; ++i) {
        this->r[i] = r_begin + i * r_step;
        this->max_infect[i] = 0;
    }
}

void WsGraph::spread() {
    for (int i = 0; i < ws_R_SAMPLE_SIZE; ++i) {
        this->adj[0]->status = 0;
        bfs_virus(0, this->r[i]);
        for (int v = 0; v < ws_N; ++v) {
            if (this->adj[v]->status == -1)
                this->max_infect[i] += 1;
            this->adj[v]->status = 1;
        }
    }
    for (int i = 0; i < ws_R_SAMPLE_SIZE; ++i) {
        if (this->max_infect[i] > ws_N / 2) {
            this->r_half = this->r[i];
            break;
        }
    }
}