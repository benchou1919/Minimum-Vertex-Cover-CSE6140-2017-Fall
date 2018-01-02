// #pragma once

#ifdef CPLEX
#include <ilcplex/ilocplex.h>
#endif
#include <vector>
#include <map>
#include <memory>
#include <ctime>
#include "Graph.h"
#include <fstream>
#include <iomanip>
#include <sstream>
#include "LS2.h"

using namespace std;

#if !defined ( BNB_H )
#define BNB_H

class BnB {
	Graph _graph;
	std::string _casename;
	
	clock_t _bnbStartTime;
	double _cutoff_time;
	std::vector<int> bestX;
	int bestObj;
	std::vector<int> solutionList;
	std::vector<double> solutiontimeList;

public:
	BnB();
	BnB(Graph g, std::string casename, double cutoff_time);
	std::vector<Edge> countUncoveredNeighbors(int v, std::vector<Edge> uncoverEdge);
	void startBnB();
	void buildRootModel();
	double matchingLB(std::vector<Edge> edge);
	void optimize();
	void writeSolution();
	void dfs(std::vector<int> assignment, std::vector<Edge> uncoveredEdge, int depth);
	int countAssignment(std::vector<int> assignment);
	static std::vector<Edge> getEdgeList(Graph g);
};



#endif
