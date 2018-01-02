/*
CSE6140 Final Project
Problem: Minimum Vertex Cover (MVC)
Team member:
	Zhuonan Li
	Shuhe Chou
	Xinyao Qian
*/

#include "RunMVC.h"

using namespace std;

void usage() {
	std::cout << "Usage: RunMVC -inst <filename> -alg [BnB|Approx|LS1|LS2] -time <cutoff in seconds> -seed <random seed>" << endl;
}


int main(int argc, char *argv[]) {

	/*
	1. inputs: graph file, algorithm type, cutoff time (in second), random seed
	*/

	std::string graph_file;
	int algorithm;	//0: BnB; 1: Approx; 2: LS1; 3: LS2
	double cutoff_time;
	double rd_seed;

	if (argc < 4) {
		usage();
		return 1;
	}

	for(int i=1;i<argc;i++){
		std::string opt(argv[i]);
		if(opt == "-inst"){
			graph_file = argv[i+1];
			i++;
		} else if(opt == "-alg"){
			std::string alg(argv[i+1]);
			if (alg == "BnB")
				algorithm = 0;
			else if (alg == "Approx")
				algorithm = 1;
			else if (alg == "LS1")
				algorithm = 2;
			else if (alg == "LS2")
				algorithm = 3;
			else {
				cout << "RunMVC does not accept " << alg << " Algorithm type." << endl;
				usage();
				return 1;
			}
			i++;
		} else if(opt == "-time"){
			std::istringstream arg(argv[i+1]);
			arg >> cutoff_time;
			i++;
		} else if (opt == "-seed"){
			std::istringstream arg(argv[i+1]);
			arg >> rd_seed;
			i++;
		} else {
			std::cout << "RunMVC dose not accpet argument " << opt << endl;
			usage();
			return 1;
		}
    }


	// cout << graph_file << " " << algorithm << " " << cutoff_time << " " << rd_seed << endl;



	//Write this function to parse edges from graph file to create your graph object
	Graph G(graph_file);
	//G.printGraph();

	LS2 LS2Instance(G, graph_file, (int)cutoff_time, (int)rd_seed);
	BnB bnbInstance(G, graph_file, cutoff_time);
	// G.printGraph();
	// cout << algorithm << endl;
	switch (algorithm){
		case 0:
			// BnB bnbInstance(G, graph_file, cutoff_time);
			bnbInstance.startBnB();
			bnbInstance.writeSolution();
			break;
        case 1:
        {
             Approx approxInstance(G, graph_file, (int)rd_seed);
             approxInstance.dfs();
            //  approxInstance.checkCoverApprox();
             approxInstance.writeSolution();
            break;
        }
		case 2:
        {
			LS1 LS1Instance(G, graph_file, (int)cutoff_time, (int)rd_seed);
			while((float)(clock() - LS1Instance.getStartTime()) / 1000000.0 < (int)cutoff_time - 1 &&
				  (float)(clock() - LS1Instance.getLastUpdate()) / 1000000.0 < 100 &&
			   	  LS1Instance.getCntDiff() < 10000) {
				LS1Instance.edgeDeletion();
				LS1Instance.hillClimb();
			}
			LS1Instance.writeSolution();

			break;
        }
        case 3:

			while((float)(clock() - LS2Instance.getStartTime()) / 1000000.0 < (int)cutoff_time - 1 &&
				  (float)(clock() - LS2Instance.getLastUpdate()) / 1000000.0 < 15 &&
				  LS2Instance.getCntDiff() < 1000) {
				LS2Instance.approx();
				LS2Instance.SimulatedAnnealing();
			}

			LS2Instance.writeSolution();

			break;
        default:
            break;
	}



	return 0;

}
