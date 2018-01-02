#include "Approx.h"

Approx::Approx() {}

Approx::Approx(Graph graph, string filename, int rand_seed) {
    _graph = graph;
    _filename = filename;
    _seed = rand_seed;
}

void Approx::helper(Graph& g, int node, vector<int>& visited, vector<int>& sol) {

    bool flag = false;
    visited[node] = 1;
    int numEdges = g.getDegree(node), tempNeighbor;

    for (int i = 0; i < numEdges; i++) {
        tempNeighbor = g.getEdges(node)[i] - 1;

        if (visited[tempNeighbor] == 0) {
            flag = true;
            helper(g, tempNeighbor, visited, sol);
        }
    }
    if (flag) {
        // if this node servers as a non-leaf node on the spanning tree, take it into a vertex cover
        if (sol[node] == 0) {
            sol[node] = 1;
        }
    }
    return;
}

void Approx::dfs() {
    // use dfs algorithm as an approximation method
    int n = _graph.getNumNode();
    bestNum = n;

    for (int i = 0; i < 10; i++) {
        ofstream trace;
        size_t pos1 = _filename.find_last_of('/');
        size_t pos2 = _filename.find_last_of('.');
        string name = _filename.substr(pos1 + 1, pos2 - pos1 - 1);
        string output_file = "output/" + name + "_Approx_" + to_string(_seed) + ".trace";
        //cout << output_file << endl;
        trace.open(output_file);

        clock_t startDFS = clock();
        //cout << "num of nodes: " << n << endl;
        vector<int> visited(n, 0);
        vector<int> sol(n, 0);
        if (_seed != -1) {
            srand(_seed);
            int startNode = rand() % n;
            cout << "start node: " << startNode << endl;
            sol[startNode] = 1;
            helper(_graph, startNode, visited, sol);
        }

        for (int i = 0; i < n; i++) {
            if (visited[i] == 0) {
                sol[i] = 1;
                helper(_graph, i, visited, sol);
            }
        }

        for (int i = 0; i < n; i++) {
            if (sol[i] == 1) {
                _solVertex.push_back(i);
            }
        }
        clock_t endDFS = clock();

        _vertexNum = _solVertex.size();
        _solTime = (float) 1000 * (endDFS - startDFS) / CLOCKS_PER_SEC;
        trace << _solTime << "," << _vertexNum << endl;
        // write trace
        if (_vertexNum < bestNum) {
            bestNum = _vertexNum;
            bestSeed = _seed;
            bestSolVertex = _solVertex;
        }
        _seed += 10;
        _solVertex.clear();
        trace.close();
    }
}

void Approx::checkCoverApprox() {
    if (_graph.checkCover(_solVertex)) {
        cout << "the answer is an actual vertex cover." << endl;
    } else {
        cout << "the answer is not an actual vertex cover." << endl;
    }
    return;
}

void Approx::writeSolution() {
    ofstream output;
    size_t pos1 = _filename.find_last_of('/');
    size_t pos2 = _filename.find_last_of('.');
    string name = _filename.substr(pos1 + 1, pos2 - pos1 - 1);
    string output_file = "output/" + name + "_Approx_" + to_string(bestSeed) + ".sol";
    cout << output_file << endl;
    output.open(output_file);
    output << bestNum << endl;
    for (int i = 0; i < bestSolVertex.size(); i++) {
        output << bestSolVertex[i] << ",";
    }
    output<< endl;
    output.close();
}
