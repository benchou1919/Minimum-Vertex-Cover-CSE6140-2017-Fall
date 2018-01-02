#pragma once
#include "Graph.h"
#include <unordered_set>
#include <stdlib.h>     /* srand, rand */
#include <fstream>

using namespace std;

class Approx {
private:
    int _seed;
    int _vertexNum;
    vector<int> _solVertex;
    float _solTime;
    Graph _graph;
    string _filename;
    int bestNum;
    int bestSeed;
    vector<int> bestSolVertex;
    
    void helper(Graph& g, int node, vector<int>& visited, vector<int>& sol);
public:
    Approx();
    Approx(Graph graph, string filename, int rand_seed);
    void dfs();
    void checkCoverApprox();
    void writeSolution();
};
