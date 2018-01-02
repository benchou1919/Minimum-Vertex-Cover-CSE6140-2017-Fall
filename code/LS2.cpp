#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <numeric>
#include <algorithm>    // std::sort
#include <vector>       // std::vector
#include <set>
#include <utility>      // std::pair
#include <unordered_set>
#include <unordered_map>
#include <boost/functional/hash.hpp>
#include <random>
#include <climits>
#include <cmath>
#include "LS2.h"
#include "LS1.h"

LS2::LS2() {}

LS2::LS2(Graph graph, std::string filename, int cutoff_time, int seed) {
    this->begin_time = clock();
    this->last_update = this->begin_time;
    this->cutoff_time = cutoff_time;
    this->seed = seed;
    this->_graph = graph;
    this->_filename = filename;
    this->cnt = 0;
    this->last_cnt = 0;
    this->N = graph.getNumNode();
    this->E = graph.getNumEdges();
    this->bestSol = INT_MAX;
    vector<int> tmp(this->N, 0);
    for(int i = 0; i < this->N; i ++)
        tmp[i] = i;
    std::unordered_set<int> tmpSet(tmp.begin(), tmp.end());
    this->bestSet = tmpSet;
    // cout << "INITIALIZE: " << this->bestSet.size() << endl;
    this->prev_cost = INT_MAX;
    std::vector<std::pair<int, int>> pairs;
    this->uncovered_edges = pairs;

    this->isCover = true;

    /*** File Pointers ***/
    filename = filename.substr(8, filename.length() - 14);
    const char *fname = filename.c_str();

    char solution_filename[100];
    char trace_filename[100];
    sprintf(solution_filename, "output/%s_LS2_%d_%d.sol", fname, cutoff_time, seed);
    sprintf(trace_filename, "output/%s_LS2_%d_%d.trace", fname, cutoff_time, seed);

    this->solution_fp = fopen(solution_filename, "w");
    this->trace_fp = fopen(trace_filename, "w");



    std::vector<int> v(graph.getNumNode());
    std::iota(std::begin(v), std::end(v), 0);
    this->VC = v;
}

std::vector<int> LS2::approx() {
    this->cnt ++;
    srand(this->seed);
    this->seed += 10;

    this->cost.clear();
    this->last_uncovered.clear();
    this->uncovered_edges.clear();
    this->prev_cost = INT_MAX;

    std::unordered_set<std::pair<int, int>, boost::hash< std::pair<int, int> > > uncovered_edges;
    std::unordered_set<int> coverSet;


    int N = this->_graph.getNumNode();
    for(int i = 0; i < N; i ++) {
        vector<int> edges = this->_graph.getEdges(i);
        for(int v : edges) {
            // i+1, v
            uncovered_edges.insert(std::make_pair(min(i+1, v), max(i+1, v)));
        }
    }

    while(!uncovered_edges.empty()) {
        // Randomly pick 1
        std::pair<int, int> edge;

        int r = rand() % uncovered_edges.size();
        std::pair<int, int> pseudo_pair(r, r);
        uncovered_edges.insert(pseudo_pair);
        std::unordered_set<std::pair<int, int>, boost::hash< std::pair<int, int> > >::iterator it;
        it = uncovered_edges.find(pseudo_pair);
        it++;

        edge = it == uncovered_edges.end() ? *(uncovered_edges.begin()) : *it;

        uncovered_edges.erase(edge);
        uncovered_edges.erase(pseudo_pair);


        this->cost[edge.first] = this->_graph.getDegree(edge.first - 1) + 1;
        this->cost[edge.second] = this->_graph.getDegree(edge.second - 1) + 1;

        vector<int> nodes;
        nodes.push_back(edge.first);
        nodes.push_back(edge.second);

        for(int node : nodes) {
            for(int neighbor : this->_graph.getEdges(node - 1)) {
                std::pair<int, int> tmp_edge = std::make_pair(min(node, neighbor), max(node, neighbor));
                if(!uncovered_edges.count(tmp_edge)) {
                    this->cost[node] --;
                    this->cost[neighbor] --;
                }
                else {
                    uncovered_edges.erase(tmp_edge);
                }

            }
            coverSet.insert(node);
        }
    }
    double elapsed = (clock() - this->begin_time) / (double)1000000.0;

    _writeTrace(elapsed, coverSet);

    vector<int> ret(coverSet.begin(), coverSet.end());
    this->VC = ret;
    return ret;
}

void LS2::SimulatedAnnealing() {

    std::unordered_set<int> coverSet(this->VC.begin(), this->VC.end());

    std::unordered_set<int> last;
    double temperature = 10000;
    double quit_temp = 0.0001;


    double elapsed = clock() - this->begin_time;
    while( (elapsed / 1000000.0) < this->cutoff_time - 1 && temperature > quit_temp) {

        while(getCost(this->uncovered_edges) == 0) {
            removeUseless(coverSet);
            elapsed = (clock() - this -> begin_time) / (double)1000000.0;
            this->tmpSet = coverSet;
            if(!isVertexCover(coverSet)) {
                temperature = 0;
                break;
            }
            if(coverSet.size() < this->bestSet.size()) {
                this->bestSet = coverSet;
            }
            _writeTrace(elapsed, coverSet);

            removeRandom(coverSet);
            last = coverSet;
            this->last_uncovered = this->uncovered_edges;
            this->last_cost = this->cost;
        }

        removeSmallest(coverSet);
        addRandom(coverSet);

        if(replace(getCost(this->uncovered_edges), getCost(this->last_uncovered), temperature)) {
            last = coverSet;
        }
        else {
            // restore to last
            coverSet = last;
            this->cost = this->last_cost;
            this->uncovered_edges = this->last_uncovered;
        }
		temperature--;
        elapsed = clock() - this -> begin_time;
    }

    // cout << "IS VERTEX COVER: " << isVertexCover(this->bestSet) << endl;
    // cout << "VERTEX COVER SIZE: " << this->bestSet.size() << endl;
}

void LS2::removeUseless(std::unordered_set<int> &coverSet) {
    vector<int> tmp(coverSet.begin(), coverSet.end());
    for(int node : tmp) {
        if(this->cost[node] == 0) {
            vector<int> edges = this->_graph.getEdges(node - 1);
            for(int v : edges) {
                this->cost[v] ++;
            }
            coverSet.erase(node);
            this->cost.erase(node);

            double elapsed = (clock() - this->begin_time) / 1000000.0;
            _writeTrace(elapsed, coverSet);
        }
    }
}

void LS2::removeSmallest(std::unordered_set<int> &coverSet) {
    vector<int> tmp(coverSet.begin(), coverSet.end());
    int min_cost = INT_MAX;
    int to_be_removed = 0;
    for(auto it = coverSet.begin(); it != coverSet.end(); it ++) {
        if(min_cost > this->cost[*it]) {
            min_cost = this->cost[*it];
            to_be_removed = *it;
        }
    }
    _checkCovered(coverSet, to_be_removed);
    coverSet.erase(to_be_removed);
    this->cost.erase(to_be_removed);
}

void LS2::addRandom(std::unordered_set<int> &coverSet) {
    // Randomly get an edge and add one of the node to the set
    if(this->uncovered_edges.size() == 0) return;

    int r = rand() % this->uncovered_edges.size();
    std::vector<std::pair<int, int>> edges(this->uncovered_edges.begin(), this->uncovered_edges.end());
    std::vector<std::pair<int, int>> new_edges;
    int v = rand() % 2 ? edges[r].first : edges[r].second;
    coverSet.insert(v);
    vector<int> neighbors = this->_graph.getEdges(v - 1);

    int cost = 0;
    for(int neighbor : neighbors) {
        if(this->cost.count(neighbor))
            this->cost[neighbor] --;
        else {
            cost ++;
        }
    }
    this->cost[v] = cost;
    // cout << "Before: " << edges.size() << endl;
    for(auto e : edges) {
        if(e.first != v && e.second != v) {
            new_edges.push_back(e);
        }
    }
    // cout << "After: " << new_edges.size() << endl;
    this->uncovered_edges = new_edges;

}

bool LS2::isVertexCover(std::unordered_set<int> &coverSet) {
    for(int i = 0; i < this->N; i ++) {
        // i + 1 because elements in coverSet range from 1 ~ N
        if(coverSet.count(i + 1))
            continue;
        for(int v : this->_graph.getEdges(i)) {
            if(!coverSet.count(v)) {
                return false;
            }
        }
    }
    return true;
}

void LS2::removeRandom(std::unordered_set<int> &coverSet) {

    int r = rand() % this -> N;

    // This is a trick to get random element in O(1)
    if(coverSet.count(r)) {
        coverSet.erase(r);
        this->cost.erase(r);
        _checkCovered(coverSet, r);
    }
    else {
        coverSet.insert(r);
        std::unordered_set<int>::iterator it = coverSet.find(r);
        it++;
        if(it == coverSet.end()) {
            cout << "This should not happen" << endl;
        }
        _checkCovered(coverSet, *it);
        coverSet.erase(it);
        this->cost.erase(*it);
        coverSet.erase(r);
    }


}

bool LS2::replace(int s1, int s2, float temperature) {
    return s1 < s2 ? true : exp(-(s1 - s2) / temperature) > (double)rand() / RAND_MAX;
}

bool LS2::_checkCovered(std::unordered_set<int> coverSet, int vertex) {
    // check if the set will still be a cover after the vertex removed
    bool ret = true;
    vector<int> edges = this->_graph.getEdges(vertex - 1);
    for(auto v : edges) {
        if(!coverSet.count(v)) {
            ret = false;
            std::pair<int, int> p = std::make_pair(min(vertex, v), max(vertex, v));

            if(find(this->uncovered_edges.begin(), this->uncovered_edges.end(), p) == this->uncovered_edges.end()) {
                this->uncovered_edges.push_back(p);
            }
        }
        else {
            if(this->cost.count(v)) {
                this->cost[v] ++;
            }
            // else {
            //     cout << "This should not happen" << endl;
            // }
        }
    }
    return ret;
}

int LS2::getCost(std::vector<pair<int, int>> s) {
    return s.size();
}

// Sort vertex by degree

clock_t LS2::getLastUpdate() {
    return this->last_update;
}

clock_t LS2::getStartTime() {
    return this->begin_time;
}

int LS2::getCntDiff() {
    return this->cnt - this->last_cnt;
}

void LS2::_writeTrace(double time_elapsed, std::unordered_set<int> coverSet) {
    // Write to the trace file.
    // if(solution < this->bestSet.size()) {
    // cout << "sizes: " << coverSet.size() << " " << this->bestSet.size() << endl;
    if(coverSet.size() < this->bestSet.size()) {

        fprintf(this->trace_fp, "%llf, %d\n", time_elapsed, coverSet.size());
        this->last_cnt = this->cnt;
        this->last_update = clock();
        this->bestSet = coverSet;
    }

    // }

}

void LS2::writeSolution() {
    // Write to the solution file:
    vector<int> vc(this->bestSet.begin(), this->bestSet.end());
    fprintf(this->solution_fp, "%d\n", vc.size());
    fprintf(this->solution_fp, "%d", vc[0]);
    for(int i = 1; i < vc.size(); i ++) {
        fprintf(this->solution_fp, ",%d", vc[i]);
    }
}
