#ifndef graph_distance_opt_hpp
#define graph_distance_opt_hpp

#include "common.hpp"
#include "cores.hpp"
#include <map>


/*! This is the function we are interested in the execution time of. */
// Here n is the number of nodes (nodes.size())
std::vector<int> graph_distance_opt(const std::vector<node> &nodes, int start)
{
	std::vector<int> distance(nodes.size(), INT_MAX); // all nodes start at infinite distance
	
	// a list of (id,dist)
	std::deque<std::pair<int,int> > todo;
	todo.push_back(std::make_pair(start,0));

	int nodeNo = start;
	map<int, std::deque<std::pair<int,int> > > todo_list;
	
	while(todo_list.size() < nodes.size() && todo_list.size() <= (CoresInformation::getCores())){
		std::pair<int,int> curr = todo.front();
		todo.pop_front();
		
		if(distance[curr.first]==INT_MAX){
			distance[curr.first]=curr.second;
			for(unsigned i=0;i<nodes[curr.first].edges.size();i++){
				todo_list[nodeNo].push_back(std::make_pair(nodes[curr.first].edges[i],curr.second+1));
			}
		}
		nodeNo++;
	}
	// above collects n nodes worth of todo lists. Now spawn n tasks to solve those with the old method.

	for(unsigned x = 0; x < todo_list.size(); x++)
	{
		// loop through the list of todos
		// spawn task graph_distance_flat(*?todo_list[x], x)
		graph_distance_flat(, );
	}
	
	return distance;
}

#endif



	// put node start,0 on the todo list
	// get front of todo list as curr
	// pop it (i.e. consume it)
	// if its distance is infinite (i.e. default) set its distance (in first case = 0).
	// go through all the nodes edges and each time push them onto the todo with the node number and the distance from previous node

	// make a new todo list for each node.
	// make a list of todo list pointers. Nodenumber, todo pointer
	// spawn 8 tasks to tackle 8 of the todo pointers at a time.
