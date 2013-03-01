#ifndef graph_distance_seq_hpp
#define graph_distance_seq_hpp

#include "common.hpp"
#include "cores.hpp"
#include <map>
#include <iostream>
#include <tbb/task.h>

class TraverseTaskSeq 
{
	
	private:
		const std::vector<node> *nodes;
		const node *nd;
		std::vector<int> *distance;
		int dis;

	public:
		TraverseTaskSeq(
			const std::vector<node> *nodes,
			const node *nd,
			std::vector<int> *distance,
			int dis
		) : nodes(nodes), nd(nd), distance(distance), dis(dis) {}

		void execute(void){
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

			for(unsigned i = 0; i < (*nd).edges.size(); i++)
			{
				// recurse through all child nodes
				TraverseTaskSeq(nodes, &(*nodes)[(*nd).edges[i]], distance, dis).execute();
			}

			return;
		}
};

/*! This is the function we are interested in the execution time of. */
// Here n is the number of nodes (nodes.size())
std::vector<int> graph_distance_seq(const std::vector<node> &nodes, int start)
{
	std::vector<int> distance(nodes.size(), INT_MAX); // all nodes start at infinite distance

	const node *start_node = &nodes[start];
	
	// Traverse the nodes starting at the start node
	TraverseTaskSeq(&nodes, start_node, &distance, -1).execute();

	return distance;
}

#endif