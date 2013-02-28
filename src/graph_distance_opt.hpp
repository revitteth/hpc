#ifndef graph_distance_opt_hpp
#define graph_distance_opt_hpp

#include "common.hpp"

/*! This is the function we are interested in the execution time of. */
// Here n is the number of nodes (nodes.size())
std::vector<int> graph_distance_opt(const std::vector<node> &nodes, int start)
{
	std::vector<int> distance(nodes.size(), INT_MAX); // all nodes start at infinite distance
	
	// a list of (id,dist)
	std::deque<std::pair<int,int> > todo;
	todo.push_back(std::make_pair(start,0));
	
	while(!todo.empty()){
		std::pair<int,int> curr = todo.front();
		todo.pop_front();
		
		if(distance[curr.first]==INT_MAX){
			distance[curr.first]=curr.second;
			for(unsigned i=0;i<nodes[curr.first].edges.size();i++){
				todo.push_back(std::make_pair(nodes[curr.first].edges[i],curr.second+1));
			}
		}
	}
	
	// put start,0 on the todo list
	// get front of todo list as curr
	// pop it (i.e. consume it)
	// if its distance is infinite (i.e. default) set its distance (in first case = 0).
	// go through all the nodes edges and each time push them onto the todo with the node number and the distance from previous node


	
	return distance;
}

#endif
