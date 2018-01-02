#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <unordered_set>
#include "Graph.h"

// using namespace std;

Graph::Graph() {
}

Graph::Graph(std::string filename) {
	std::ifstream fin(filename.c_str());
	fin >> N >> E >> directed;
	std::cout << N << " " << E << " " << directed << std::endl;
	std::string oneLine = "";
	// this->N = N;
	getline(fin, oneLine); // run to second line

	for (int i=0; i<N; i++) {
	//	cout << i << endl;
		std::vector<int> tmpLinkList;

		getline(fin, oneLine);
	//	cout << oneLine << endl;
		std::istringstream line(oneLine);
		int j = 0;
		while (line >> j) {
			// cout << j << " ";
			tmpLinkList.push_back(j);
		}
		// cout << endl;
		linkList.push_back(tmpLinkList);
	}
}

std::vector<int> Graph::getEdges(int u) {
    return linkList[u];
}

// CHECK IF the vector is truly a vertex cover
bool Graph::checkCover(std::vector<int> cover) {
    std::vector<bool> vertexCover(N, false);

    int i, j;
    for (i = 0; i < cover.size(); i++) {
        vertexCover[cover[i]] = true;
        for (j = 0; j < linkList[cover[i]].size(); j++) {
            vertexCover[linkList[cover[i]][j]] = true;
        }
    }

    for (i = 0; i < N; i++) {
        if (vertexCover[i] == false) {
            return false;
        }
    }

    return true;
}

bool Graph::checkCoverBinary(std::vector<int> cover) {
	// std::unordered_set<int> visited;
	// for(int i = 0; i < N; i ++) {
	// 	if(cover[i] == 1) {
	// 		visited.insert(i);
	// 	}
	// }

	for(int i = 0; i < N; i ++) {
		if(cover[i]) {
			continue;
		}
		for(int j = 0; j < linkList[i].size(); j ++) {

			if(!cover[linkList[i][j] - 1]) {
				// std::cout << linkList[i][j] << " ";
				return false;
			}
			// std::cout << std::endl;
		}
	}
	return true;
}

int Graph::getDegree(int u) {
	return linkList[u].size();
}

std::vector<int> Graph::getNeighbors(int node) {
    return linkList[node];
}

int Graph::getNumNode() {
	return N;
}

int Graph::getNumEdges() {
	return E;
}

void Graph::printGraph() {
	for (int i=0; i<N; i++) {
		for (int j=0; j<linkList[i].size(); j++)
			std::cout << linkList[i][j] << " ";
		std::cout << std::endl;
	}
}


Edge::Edge() {
}

Edge::Edge(int v1, int v2) {
	this->v1 = v1;
	this->v2 = v2;
}

bool Edge::contains(int v) {
	if (this->v1 == v || this->v2 == v)
		return true;
	else
		return false;
}
