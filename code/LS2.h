#pragma once
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <time.h>       /* time */
#include "Graph.h"

using namespace std;

class LS2 {
    private:
        int seed;
        clock_t begin_time;
        clock_t last_update;
        int cutoff_time;
        int cnt;
        int last_cnt;
        int prev_cost;
        int N;
        int E;
        bool isCover;
        int bestSol;

        std::vector<int> VC;
        std::unordered_map<int, int> cost;
        std::unordered_set<int> bestSet;
        std::unordered_set<int> tmpSet;
        std::vector<std::pair<int, int>> uncovered_edges;
        std::vector<std::pair<int, int>> last_uncovered;
        std::unordered_map<int, int> last_cost;

        Graph _graph;
        std::string _filename;
        FILE* solution_fp;
        FILE* trace_fp;

    public:
        LS2();
        LS2(Graph graph, std::string filename, int cutoff_time, int seed);
        // Get initial approximation vertex cover
        std::vector<int> approx();
        bool _checkCovered(std::unordered_set<int> coverSet, int vertex);
        void addRandom(std::unordered_set<int> &coverSet);
        bool replace(int s1, int s2, float temperature);
        void SimulatedAnnealing();
        void writeSolution();
        bool isVertexCover(std::unordered_set<int> &coverSet);
        void removeRandom(std::unordered_set<int> &coverSet);
        void removeSmallest(std::unordered_set<int> &coverSet);
        void removeUseless(std::unordered_set<int> &coverSet);
        clock_t getLastUpdate();
        clock_t getStartTime();
        int getCntDiff();
        int getCost(std::vector<pair<int, int>> s);
        void _writeTrace(double elapsed_time, std::unordered_set<int> coverSet);
};
