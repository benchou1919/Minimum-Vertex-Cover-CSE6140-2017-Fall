#include "BnB.h"

using namespace std;

// ILOSTLBEGIN

// IloEnv _env;

std::vector<std::vector<int> > bnbTree;
// std::vector<int> depth;
std::ofstream foutlog;

BnB::BnB() {
}

BnB::BnB(Graph g, std::string casename, double cutoff_time) {
	_graph = g;
	_casename = casename.substr(8, casename.length() - 14);
	_bnbStartTime = clock();
	_cutoff_time = cutoff_time;
	bestX.assign(_graph.getNumNode(), -1);
	bestObj = _graph.getNumNode();
	std::ostringstream cutoff;
	cutoff << cutoff_time;
	std::string logfile = "output/"+_casename+"_BnB_"+cutoff.str()+".log";
	foutlog.open(logfile.c_str(),std::ofstream::out);
}

void BnB::startBnB() {
	// optimize();

	// heuristic
	// LS2 LS2Instance(_graph, _casename, _cutoff_time, 3);
	// std::vector<int> initialSol = LS2Instance.approx();
	// bestObj = initialSol.size();
	// solutionList.push_back(bestObj);
	// solutiontimeList.push_back(((double)(clock()-_bnbStartTime)/CLOCKS_PER_SEC));
	// std::vector<int> initialAssignment(_graph.getNumNode(), 0);
	// for (int i=0; i<initialSol.size(); i++)
	// 	initialAssignment[initialSol[i]-1] = 1;
	// for (int i=0; i<_graph.getNumNode(); i++)
	// 	bestX[i] = initialAssignment[i];

	// start bnb
	foutlog << "start" << endl;
	std::vector<int> assignment(_graph.getNumNode(), -1);
	std::vector<Edge> uncoveredEdge;
	uncoveredEdge = getEdgeList(_graph);
	for (int i=0; i<uncoveredEdge.size(); i++)
		foutlog << uncoveredEdge[i].v1 << " " << uncoveredEdge[i].v2 << "; ";
	foutlog << endl;
	foutlog << "start dfs" << endl;
	dfs(assignment, uncoveredEdge, 0);
}

std::vector<Edge> BnB::getEdgeList(Graph g) {
	std::vector<Edge> list;
	for (int i=0; i<g.getNumNode(); i++) {
		std::vector<int> neighbors = g.getEdges(i);
		for (int j=0; j<neighbors.size(); j++) {
			if (i<neighbors[j]) {
				list.push_back(Edge(i,neighbors[j]-1));
			}
		}
	}
	return list;
}



void BnB::dfs(std::vector<int> assignment, std::vector<Edge> uncoveredEdge, int depth) {
	// foutlog << depth << endl;

	if (((float)(clock()-_bnbStartTime)/CLOCKS_PER_SEC) > _cutoff_time) {
		writeSolution();
		exit(0);
	}

	int currentAssignmentCount = countAssignment(assignment);

	// already not the best
	if (currentAssignmentCount > bestObj) {
		foutlog << "pruned" << endl;
		return;
	}


	// current + remain lower bound > bestObj
	foutlog << "current + lb " << currentAssignmentCount << " " << matchingLB(uncoveredEdge) << " best " << bestObj << endl;
	if (currentAssignmentCount + matchingLB(uncoveredEdge) > bestObj) {
		foutlog << "pruned" << endl;
		return;
	}

	// check infeasible
	for (int i=0; i<uncoveredEdge.size(); i++)
		if (assignment[uncoveredEdge[i].v1] == 0 && assignment[uncoveredEdge[i].v2] == 0) {
			foutlog << "infeasible" << endl;
			return;
		}


	// have a vertex cover
	if (uncoveredEdge.size() == 0) {
		// a better than bestObj vertex cover
		if (currentAssignmentCount < bestObj) {
			bestObj = currentAssignmentCount;
			solutionList.push_back(currentAssignmentCount);
			solutiontimeList.push_back(((double)(clock()-_bnbStartTime)/CLOCKS_PER_SEC));
			for (int i=0; i<_graph.getNumNode(); i++)
				bestX[i] = (int) assignment[i];
		}
		foutlog << " cover "<< endl;
		return;
	}

	// std::vector<Edge> uncoveredEdge;
	// uncoveredEdge =

	int branchingV;
	int tmpMax = 0;
	std::vector<Edge> neighborEdges;
	for (int i=0; i<_graph.getNumNode(); i++) {
		if (assignment[i] == -1) {
			std::vector<Edge> tmp = countUncoveredNeighbors(i, uncoveredEdge);
			// foutlog << i << " " << tmp.size() << endl;
			if (tmp.size() > tmpMax) {
				tmpMax = tmp.size();
				branchingV = i;
				neighborEdges = tmp;
			}
		}
	}

	// select branchingV
	for (int i=uncoveredEdge.size()-1; i>=0; i--) {
		if (uncoveredEdge[i].contains(branchingV))
			uncoveredEdge.erase(uncoveredEdge.begin()+i);
	}
	assignment[branchingV] = 1;

	// foutlog << "branch on " << branchingV << endl;
	// for (int i=0; i<uncoveredEdge.size(); i++)
	// 	foutlog << uncoveredEdge[i].v1 << " " << uncoveredEdge[i].v2 << "; ";
	// foutlog << endl;

	dfs(assignment, uncoveredEdge, depth+1);

	// not select branchingV
	for (int i=0; i<neighborEdges.size(); i++)
		uncoveredEdge.push_back(neighborEdges[i]);
	assignment[branchingV] = 0;
	// for (int i=0; i<uncoveredEdge.size(); i++)
	// 	foutlog << uncoveredEdge[i].v1 << " " << uncoveredEdge[i].v2 << "; ";
	// foutlog << endl;
	dfs(assignment, uncoveredEdge, depth+1);

}

int BnB::countAssignment(std::vector<int> assignment) {
	int count = 0;
	for (int i=0; i<assignment.size(); i++)
		if (assignment[i] >= 1)
			count ++;
	return count;
}


std::vector<Edge> BnB::countUncoveredNeighbors(int v, std::vector<Edge> uncoveredEdge) {
	std::vector<Edge> uncoveredNeighbors;
	for (int i=0; i<uncoveredEdge.size(); i++)
		if (uncoveredEdge[i].contains(v))
			uncoveredNeighbors.push_back(uncoveredEdge[i]);
	return uncoveredNeighbors;
}

double BnB::matchingLB(std::vector<Edge> edge) {
	int count = 0;
	while (edge.size() > 0) {
		count ++;
		int v1 = edge[0].v1;
		int v2 = edge[0].v2;
		for (int i=edge.size()-1; i>=0; i--)
			if (edge[i].contains(v1) || edge[i].contains(v2))
				edge.erase(edge.begin()+i);
	}
	return count/2;
}

void BnB::writeSolution() {
	std::ostringstream cutoff_time;
	cutoff_time << _cutoff_time;
	std::string solfile = "Output/"+_casename+"_BnB_"+cutoff_time.str()+".sol";
	std::string tracefile = "Output/"+_casename+"_BnB_"+cutoff_time.str()+".trace";
	ofstream fout1(solfile.c_str());
	ofstream fout2(tracefile.c_str());
	fout1 << solutionList[solutionList.size()-1] << endl;
	for (int i=0; i<_graph.getNumNode(); i++)
		if (bestX[i] > 0)
			fout1 << i+1 << ",";
	for (int i=0; i<solutionList.size(); i++) {
		fout2 << solutiontimeList[i] << ", " << solutionList[i] << endl;
	}

}



// void BnB::optimize() {
// 	foutlog << "start build root" << endl;
// 	try {
// 		IloModel model(_env);
// 		IloNumVarArray var(_env);
// 		IloRangeArray rng(_env);
// 		IloExpr objexpr(_env);

// 		// add variables
// 		for (int i=0; i<_graph.getNumNode(); i++) {
// 			var.add(IloNumVar(_env, 0.0, 1.0));
// 			objexpr += 1*var[i];
// 		}

// 		// add objective function
// 		model.add(IloMinimize(_env,objexpr));

// 		// add constraints for each edge
// 		for (int i=0; i<_graph.getNumNode(); i++) {
// 			for (int j=0; j<_graph.linkList[i].size(); j++) {
// 				if (i<_graph.linkList[i][j]) {
// 					rng.add(IloRange(_env,1.0,var[i]+var[_graph.linkList[i][j]-1]));
// 				}
// 			}
// 		}
// 		model.add(rng);

// 		IloCplex cplex(model);

// 		// cplex.exportModel("./root.lp");

// 		// IloRange cut(_env,2.0,var[0]+var[9]);
// 		// model.add(cut);

// 		// cplex.exportModel("./root2.lp");

// 		// cut.removeFromAll();

// 		// cplex.exportModel("./root3.lp");

// 		cplex.setParam(IloCplex::Param::RootAlgorithm, IloCplex::Barrier);
// 		cplex.setOut(_env.getNullStream());

// 		// foutlog << "root LP relaxation solution: " << cplex.getObjValue() << endl;

// 		std::vector<int> treenode(_graph.getNumNode(), -1);
// 		bnbTree.push_back(treenode);
// 		depth.push_back(0);

// 		while (bnbTree.size() > 0) {
// 			for (int i=0; i<_graph.getNumNode(); i++)
// 				foutlog << bnbTree[0][i] << " ";
// 			foutlog << endl;

// 			IloRangeArray cuts(_env);
// 			//add cuts
// 			for (int i=0; i<_graph.getNumNode(); i++) {
// 				switch (bnbTree[0][i]) {
// 					case -1: break;
// 					case 0: cuts.add(IloRange(_env, 0.0, var[i], 0.0)); break;
// 					case 1: cuts.add(IloRange(_env, 1.0, var[i])); break;
// 				}
// 			}

// 			model.add(cuts);
// 			// obtain lower bound by solving LP
// 			if (!cplex.solve()) {
// 				foutlog << "failed to optimize mip" << std::endl;
// 				// throw(-1);
// 			} else {
// 				foutlog << "LP relaxation solution: " << cplex.getObjValue() << endl;


// 				// get solution vector
// 				IloNumArray solutionX(_env);
// 				double objValue = cplex.getObjValue();
// 				foutlog << objValue << endl;
// 				cplex.getValues(solutionX, var);
// 				foutlog << "solution: ";
// 				for (int i=0; i<_graph.getNumNode(); i++)
// 					foutlog << solutionX[i] << " ";
// 				foutlog << endl;

// 				foutlog << "start pruning" << endl;
// 				bool prune = false;
// 				if (solutionList.size() > 0) {
// 					foutlog << "current best: " << solutionList[solutionList.size()-1] << endl;
// 					if (objValue > solutionList[solutionList.size()-1])
// 						prune = true;
// 				}

// 				if (!prune) {
// 					foutlog << "not pruned, start check integrality" << endl;
// 					// check integrality
// 					bool integer = true;
// 					for (int i=0; i<_graph.getNumNode(); i++)
// 						if (solutionX[i]-((int) solutionX[i]) != 0) {
// 							integer = false;
// 							break;
// 						}


// 					if (integer) { // add better integer solution
// 						foutlog << "Integer!" << endl;
// 						solutionList.push_back((int)objValue);
// 						solutiontimeList.push_back(((double)(clock()-_bnbStartTime)/CLOCKS_PER_SEC));

// 						foutlog << solutiontimeList[solutiontimeList.size()-1] << ", " << objValue << endl;
// 						for (int i=0; i<_graph.getNumNode(); i++)
// 							bestX[i] = (int) solutionX[i];
// 						foutlog << "finish updating" << endl;
// 					} else { // add branches
// 						foutlog << "Not Integer, start branching" << endl;
// 						// find branching variable
// 						double minTmp = 100;
// 						int branchingVar;
// 						// for (int i=0; i<_graph.getNumNode(); i++)
// 						// 	if (fabs(solutionX[i]-0.5) < minTmp) {
// 						// 		minTmp = fabs(solutionX[i]-0.5);
// 						// 		branchingVar = i;
// 						// 	}

// 							// } else if (fabs(solutionX[i]-0.5) == minTmp) {
// 							// 	if (_graph.linkList[i].size() > )
// 							// }

// 						int maxUncoveredNeighbor = 0;
// 						for (int i=0; i<_graph.getNumNode(); i++)
// 							if (bnbTree[0][i] == -1 && solutionX[i] <1 && solutionX[i] > 0){
// 								int count = countUncoveredNeighbors(i, bnbTree[0]);
// 								if ( count > maxUncoveredNeighbor ) {
// 									branchingVar = i;
// 									maxUncoveredNeighbor = count;
// 								}
// 							}

// 						// branch on i
// 						std::vector<int> leftchild(bnbTree[0]), rightchild(bnbTree[0]);
// 						leftchild[branchingVar] = 0;
// 						rightchild[branchingVar] = 1;
// 						bnbTree.push_back(rightchild);
// 						depth.push_back(depth[0]+1);
// 						bnbTree.push_back(leftchild);
// 						depth.push_back(depth[0]+1);
// 						foutlog << "Branch on " << branchingVar << endl;

// 						if (depth[0] % 1 == 0) { // start heuristic
// 							foutlog << "Heuristic" << endl;
// 							int objVal_Heuristic = 0;
// 							for (int i=0; i<_graph.getNumNode(); i++)
// 								if (solutionX[i] >= 0.5)
// 									objVal_Heuristic += 1;
// 							bool better = true;
// 							if (solutionList.size() > 0)
// 								if (objVal_Heuristic > solutionList[solutionList.size()-1])
// 									better = false;
// 							if (better) {
// 								solutionList.push_back(objVal_Heuristic);
// 								foutlog << "solution LIst: ";
// 								for (int i=0; i<solutionList.size(); i++) {
// 									foutlog << solutionList[i] << " ";
// 								}
// 								foutlog << endl;
// 								solutiontimeList.push_back(((double)(clock()-_bnbStartTime)/CLOCKS_PER_SEC));
// 								for (int i=0; i<_graph.getNumNode(); i++)
// 									if (solutionX[i] >= 0.5)
// 										bestX[i] = 1;
// 									else
// 										bestX[i] = 0;
// 								foutlog << "Heuristic finds a better solution: " << objVal_Heuristic << endl;
// 							}
// 						}
// 					}
// 				} else {
// 					foutlog << "got pruned" << endl;
// 				}
// 			}

// 			cuts.removeFromAll();


// 			bnbTree.erase(bnbTree.begin());
// 			depth.erase(depth.begin());
// 			if (((float)(clock()-_bnbStartTime)/CLOCKS_PER_SEC) > _cutoff_time)
// 				break;
// 		}

// 	}
// 	catch (IloException& e) {
// 		// std::cerr << "Concert exception caught:" << e << std::endl;
// 	}
// 	catch (...) {
// 		// std::cerr << "Unknown exception caught:" << std::endl;
// 	}
// }
