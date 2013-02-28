#ifndef graph_distance_opt_hpp
#define graph_distance_opt_hpp

#include "common.hpp"
#include "cores.hpp"
#include <map>
#include <iostream>

void treeTraverse(const std::vector<node> *nodes, const node *nd, std::vector<int> *distance, int dis)
{
	dis++;

	if((*distance)[(*nd).id] == INT_MAX)
	{
		(*distance)[(*nd).id] = dis;
	}
	else if ((*distance)[(*nd).id] > dis)
	{
		(*distance)[(*nd).id] = dis;
	}
	else 
	{
		dis--;
		return;
	}

	for(int i = 0; i < (*nd).edges.size(); i++)
	{
		treeTraverse(nodes, &(*nodes)[(*nd).edges[i]], distance, dis);
	}
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