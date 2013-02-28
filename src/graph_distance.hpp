#ifndef graph_distance_hpp
#define graph_distance_hpp

#include "common.hpp"

/*! This is the function we are interested in the execution time of. */
// Here n is the number of nodes (nodes.size())
std::vector<int> graph_distance(const std::vector<node> &nodes, int start)
{
	std::vector<int> distance(nodes.size(), INT_MAX); // all nodes start at infinite distance
	
	// a list of (id,dist)
	std::deque<std::pair<int,int> > todo;
	todo.push_back(std::make_pair(start,0));
	
	while(!todo.empty()){
		std::pair<int,int> curr=todo.front();
		todo.pop_front();
		
		if(distance[curr.first]==INT_MAX){
			distance[curr.first]=curr.second;
			for(unsigned i=0;i<nodes[curr.first].edges.size();i++){
				todo.push_back(std::make_pair(nodes[curr.first].edges[i],curr.second+1));
			}
		}
	}
	
	return distance;
}

#endif
