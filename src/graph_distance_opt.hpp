#ifndef graph_distance_opt_hpp
#define graph_distance_opt_hpp

#include "common.hpp"
#include "cores.hpp"
#include <map>
#include <iostream>
#include <tbb/task.h>

void treeTraverse(const std::vector<node> *nodes, const node *nd, std::vector<int> *distance, int dis);

class TraverseTask : public tbb::task {
	
	private:
		const std::vector<node> *nodes;
		const node *nd;
		std::vector<int> *distance;
		int dis;

	public:
		TraverseTask(
			const std::vector<node> *nodes,
			const node *nd,
			std::vector<int> *distance,
			int dis
		) : nodes(nodes), nd(nd), distance(distance), dis(dis) {}

		tbb::task * execute(void){
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
				return NULL;
			}

			tbb::task_list tList;

			for(int i = 0; i < (*nd).edges.size(); i++)
			{
				tList.push_back(*new (tbb::task::allocate_child()) TraverseTask(nodes, &(*nodes)[(*nd).edges[i]], distance, dis));
			}
			
			set_ref_count((*nd).edges.size()+1);
			tbb::task::spawn_and_wait_for_all(tList);

			return NULL;
		}
};

/*! This is the function we are interested in the execution time of. */
// Here n is the number of nodes (nodes.size())
std::vector<int> graph_distance_opt(const std::vector<node> &nodes, int start)
{
	std::vector<int> distance(nodes.size(), INT_MAX); // all nodes start at infinite distance

	const node *start_node = &nodes[start];
	
	//treeTraverse(&nodes, start_node, &distance, -1);
	TraverseTask &root = *new(tbb::task::allocate_root()) TraverseTask(&nodes, start_node, &distance, -1);
	tbb::task::spawn_root_and_wait(root);

	return distance;
}

#endif