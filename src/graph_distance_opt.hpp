#ifndef graph_distance_opt_hpp
#define graph_distance_opt_hpp

#include "common.hpp"
#include "cores.hpp"
#include <map>
#include <iostream>

//int dis = 0;

void treeTraverse(const std::vector<node> *nodes, const node *nd, std::vector<int> *distance, int dis)
{
	// increase recursion level (and distance from start)
	dis++;

	bool stop = false;
	// stop recursing when all nodes edge nodes don't have Int_max
	for (int h = 0; h < (*nd).edges.size(); h++)
	{
		int id = (*nd).edges[h];
		//std::cout << id << " :: :: " << (*distance)[id] << " :: :: " << stop << std::endl;
		if((*distance)[id] != INT_MAX && (*distance)[(*nd).id] != INT_MAX)
		{
			stop = true;
		}
	}

	if(!stop)
	{
		(*distance)[(*nd).id] = dis;
		// foreach node set its distance as the recursion depth.
		for(int i = 0; i < (*nd).edges.size(); i++)
		{
			treeTraverse(nodes, &(*nodes)[(*nd).edges[i]], distance, dis);
		}
	}
	return;
}

/*! This is the function we are interested in the execution time of. */
// Here n is the number of nodes (nodes.size())
std::vector<int> graph_distance_opt(const std::vector<node> &nodes, int start)
{
	std::vector<int> distance(nodes.size(), INT_MAX); // all nodes start at infinite distance

	const node *start_node = &nodes[start];
	
	treeTraverse(&nodes, start_node, &distance, -1);
	
	return distance;
}

#endif