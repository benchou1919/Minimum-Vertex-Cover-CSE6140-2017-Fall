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
#include <random>
#include "LS1.h"

LS1::LS1() {}

LS1::LS1(Graph graph, std::string filename, int cutoff_time, int seed) {
    this->begin_time = clock();
    this->last_update = this->begin_time;
    this->cutoff_time = cutoff_time;
    this->seed = seed;
    this->_graph = graph;
    this->_filename = filename;
    this->cnt = 0;
    this->last_cnt = 0;

    /*** File Pointers ***/
    // Solution file: store the best solution.
    // trace file: store the trace each time we find a better solution
    filename = filename.substr(8, filename.length() - 14);
    const char *fname = filename.c_str();

    char solution_filename[100];
    char trace_filename[100];
    sprintf(solution_filename, "output/%s_LS1_%d_%d.sol", fname, cutoff_time, seed);
    sprintf(trace_filename, "output/%s_LS1_%d_%d.trace", fname, cutoff_time, seed);

    this->solution_fp = fopen(solution_filename, "w");
    this->trace_fp = fopen(trace_filename, "w");



    std::vector<int> v(graph.getNumNode());
    std::iota(std::begin(v), std::end(v), 0);
    // Vertex Cover
    this->VC = v;

    /*** Record the best so far ***/
    this->bestVC = v;
}

// Approximation algorithm to initialize a valid vertex cover
// and add it to this->bestVC
void LS1::edgeDeletion() {

    std::unordered_set<int> visited;
    int N = this->_graph.getNumNode();

    // Number of times called.
    this->cnt += 1;
    // Generate 0 ~ N-1
    vector<int> randoms(N, 0);
    for(int i = 0; i < N; i ++) {
        randoms[i] = i;
    }

    // Shuffle by seed
    shuffle(randoms.begin(), randoms.end(), std::default_random_engine(this->seed));
    this->seed += 10;

    // Pick an edge (u, v) each time, delete all the edges connected
    // to either u or v.
    // repeat until no edge left
    for(int i = 0; i < N; i ++) {
        int u = randoms[i];

        // Visited means deleted, which means all the edges connected to it
        // can be ignored
        if(visited.count(u + 1) > 0){
            continue;
        }
        vector<int> edges = this->_graph.getEdges(u);

        for(int v : edges) {
            // search until there is an edge that both ends are not deleted
            if(visited.count(v) > 0)
                continue;
            visited.insert(u + 1);
            visited.insert(v);
            break;
        }
    }
    std::vector<int> v;
    v.insert(v.end(), visited.begin(), visited.end());
    this->VC = v;
}

vector<int> LS1::hillClimb() {
    // Sort vertices by their degrees
    vector<int> sortedCover = _sortCover();

    // Construct a set
    std::unordered_set<int> coverSet(sortedCover.begin(), sortedCover.end());
    clock_t elapsed = clock() - this->begin_time;
    for(int i = 0; i < sortedCover.size() && (elapsed / 1000000.0) < this->cutoff_time - 1; i ++) {
        elapsed = clock() - this -> begin_time;
        // delete the vertex if it will remain a vertex cover after deleting the vertex
        if(_checkCovered(coverSet, sortedCover[i])) {
            coverSet.erase(sortedCover[i]);
            _writeTrace(elapsed / 1000000.0, coverSet.size());
        }
    }
    std::vector<int> tmp(coverSet.begin(), coverSet.end());
    this->VC = tmp;

    // Update
    if(this->bestVC.size() > coverSet.size()) {
        this->bestVC = this->VC;
        this->last_update = clock();
        this->last_cnt = this->cnt;
        _writeTrace((float)elapsed / 1000000.0, (int)coverSet.size());
    }
    return this->VC;
}


bool LS1::_checkCovered(std::unordered_set<int> coverSet, int vertex) {
    // check if the set will still be a cover after the vertex removed
    vector<int> edges = this->_graph.getEdges(vertex - 1);
    for(auto v : edges) {
        if(!coverSet.count(v)) {
            return false;
        }
    }
    return true;
}

// Sort vertex by degree
std::vector<int> LS1::_sortCover() {
    std::vector<std::pair<int, int>> vertexCoverPairs;
    for(int u : this->VC) {
        vertexCoverPairs.push_back(std::make_pair(u, this->_graph.getDegree(u - 1)));
    }
    std::sort(vertexCoverPairs.begin(), vertexCoverPairs.end(),
                [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
                    return a.second < b.second;
                }
             );
    std::vector<int> ret;
    for(auto pair : vertexCoverPairs) {
        ret.push_back(pair.first);
    }
    return ret;
}

clock_t LS1::getLastUpdate() {
    return this->last_update;
}

clock_t LS1::getStartTime() {
    return this->begin_time;
}

int LS1::getCntDiff() {
    return this->cnt - this->last_cnt;
}

void LS1::_writeTrace(float time_elapsed, int solution) {
    // Write to the trace file.
    if(solution < this->bestVC.size())
        fprintf(this->trace_fp, "%f, %d\n", time_elapsed, solution);
}

void LS1::writeSolution() {
    // Write to the solution file:
    fprintf(this->solution_fp, "%d\n", (int)this->bestVC.size());
    fprintf(this->solution_fp, "%d", this->bestVC[0]);
    for(int i = 1; i < this->bestVC.size(); i ++) {
        fprintf(this->solution_fp, ",%d", this->bestVC[i]);
    }
}
