#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <iomanip>

#if !defined( GRAPH_H )
#define GRAPH_H

class Graph { // the graph stored as a linked List
    std::vector<std::vector<int> > linkList;
    int N, E, directed;

public:
	Graph();
    Graph(std::string);
    void printGraph();
    int getNumNode();
    int getNumEdges();
    int getDegree(int u);
    std::vector<int> getNeighbors(int node);
    bool checkCover(std::vector<int> cover);
    bool checkCoverBinary(std::vector<int> cover);
    std::vector<int> getEdges(int u);
};

class Edge {
public:
    int v1, v2;
    Edge();
    Edge(int v1, int v2);
    bool contains(int v);
};

#endif
