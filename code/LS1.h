#pragma once
#include <vector>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <time.h>       /* time */
#include "Graph.h"

using namespace std;

class LS1 {
    private:
        int seed;
        clock_t begin_time;
        clock_t last_update;
        int cutoff_time;
        int cnt;
        int last_cnt;
        int vertexNum;
        vector<int> VC;
        vector<int> bestVC;
        double solTime;
        Graph _graph;
        std::string _filename;
        FILE* solution_fp;
        FILE* trace_fp;
    public:
        LS1();
        LS1(Graph graph, std::string filename, int cutoff_time, int seed);
        void edgeDeletion();
        std::vector<int> _sortCover();
        bool _checkCovered(std::unordered_set<int> coverSet, int vertex);
        vector<int> hillClimb();
        void writeSolution();
        clock_t getLastUpdate();
        clock_t getStartTime();
        int getCntDiff();
        void _writeTrace(float elapsed_time, int solution);
};
