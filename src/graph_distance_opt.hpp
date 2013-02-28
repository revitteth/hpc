#ifndef graph_distance_opt_hpp
#define graph_distance_opt_hpp

#include "common.hpp"
#include "cores.hpp"
#include <map>

void treeTraverse(const std::vector<node> *nodes, const node *nd, std::vector<node> *done, std::vector<int> *distance, int dis, int start)
{
	// *nd is the nodes
	// distance is the vector of distances (initialised to maxint)
	// dis is the number of recursive calls (i.e. distance from start)
	// start is the starting node number

	// increase recursion level (and distance from start)
	dis++;

	if((*done).size() == (*nodes).size())
	{
		// stop recursing
		return;
	}
	else
	{
		(*done).push_back(*nd);
		// foreach node set its distance as the recursion depth.
		for(int i = 0; i < (*nd).edges.size(); i++)
		{
			(*distance)[(*nd).id] = dis;
			treeTraverse(nodes, &(*nodes)[i], &(*done), distance, dis, start);
		}
	}


	
}

/*! This is the function we are interested in the execution time of. */
// Here n is the number of nodes (nodes.size())
std::vector<int> graph_distance_opt(const std::vector<node> &nodes, int start)
{
	std::vector<int> distance(nodes.size(), INT_MAX); // all nodes start at infinite distance
	std::vector<node> done(nodes.size());
	
	treeTraverse(&nodes, &nodes[0], &done, &distance, -1, start);
	
	return distance;
}

#endif