#include "graph_distance.hpp"
#include "graph_distance_tbb.hpp"
#include "graph_distance_opt.hpp"
#include "graph_distance_seq.hpp"

#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <iostream>
#include <tbb/tbb.h>
#include "cores.hpp"

using namespace std;
using namespace tbb;

int CoresInformation::coresNo;

int main(int argc, char *argv[])
{
	if(argc<3){
		std::cerr<<"Must specify n and number of cpus.\n";
		// hold open terminal
		std::cin.get();
		return 1;
	}
	int n=atoi(argv[1]);
	CoresInformation::setCores(atoi(argv[2]));
	int count = 1;
	if(argv[3])
	{
		count = atoi(argv[3]);
	}
	
	// build the graph
	std::vector<node> graph = build_graph(n);

	double time_orig(0), time_tbb(0), time_opt(0), time_seq(0);
	int count_orig, count_tbb, count_opt, count_seq;
	count_orig = count_tbb = count_opt = count_seq = count;

	// The run-time can vary, depending on where you start from. How should you
	// take that into account when timing it? (use same start for each algorithm)
	int start=rand()%n;

	// original
	std::vector<int> tmp;
	for(int i = 0; i < count; i++)
	{
		tick_count start_orig = tick_count::now();
		tmp=graph_distance(graph, start);
		tick_count end_orig = tick_count::now();
		time_orig += (end_orig-start_orig).seconds();
	}
	cout << time_orig/count << endl;


	// tbb enabled
	std::vector<int> tmp_tbb;
	for(int i = 0; i < count; i++)
	{
		tick_count start_tbb = tick_count::now();
		tmp_tbb = graph_distance_tbb(graph, start);
		tick_count end_tbb = tick_count::now();
		time_tbb += (end_tbb-start_tbb).seconds();
	}

	// Check for output errors
	for(unsigned i=0;i<tmp_tbb.size();i++){
		if(tmp_tbb[i] != tmp[i]) { std::cout << "tbb error" << endl; }
	}
	cout << time_tbb/count << endl;


	// tbb optimised
	std::vector<int> tmp_opt;
	for(int i = 0; i < count; i++)
	{
		tick_count start_opt = tick_count::now();
		tmp_opt = graph_distance_opt(graph, start);
		tick_count end_opt = tick_count::now();
		time_opt += (end_opt-start_opt).seconds();
	}

	// Check for output errors
	for(unsigned i=0;i<tmp_opt.size();i++){
		if(tmp_opt[i] != tmp[i]) { std::cout << "opt error" << endl; }
	}
	cout << time_opt/count << endl;


	// sequential
	std::vector<int> tmp_seq;
	for(int i = 0; i < count; i++)
	{
		tick_count start_seq = tick_count::now();
		tmp_seq = graph_distance_seq(graph, start);
		tick_count end_seq = tick_count::now();
		time_seq += (end_seq-start_seq).seconds();
	}

	// Check for output errors
	for(unsigned i=0;i<tmp_opt.size();i++){
		if(tmp_seq[i] != tmp[i]) { std::cout << "seq error" << endl; }
	}
	cout << time_seq/count << endl;


	// CHECK OUTPUTS!!!
	//for(int i=0;i<tmp.size();i++){
		//fprintf(stdout, "dist(%d->%d) = %d\n", start, i, tmp[i]);
	//}
	
	//dump_graph(graph);

	//std::cin.get();
	
	return 0;
}
