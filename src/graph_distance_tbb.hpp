#ifndef graph_distance_tbb_hpp
#define graph_distance_tbb_hpp

#include "common.hpp"
#include <tbb/tbb.h>

using namespace tbb;

struct ParDo {
	private :
		std::vector<int>* distance;
		std::vector<node> nodes;
		std::deque<std::pair<int,int> > *todo;

	public :
		ParDo(std::vector<int>* distance, std::vector<node> nodes, std::deque<std::pair<int,int> > *todo) : distance(distance), nodes(nodes), todo(todo) {};

		void operator() (std::pair<int,int> *t, parallel_do_feeder<std::deque<std::pair<int,int> > > &feeder) const
		{
			std::pair<int,int> curr=(*t);
			
			if((*distance)[curr.first]==INT_MAX){
				(*distance)[curr.first]=curr.second;
				for(unsigned i=0;i<nodes[curr.first].edges.size();i++){
					(*todo).push_back(std::make_pair(nodes[curr.first].edges[i],curr.second+1));
				}
			}	
		}
		 
};

/*! This is the function we are interested in the execution time of. */
// Here n is the number of nodes (nodes.size())
std::vector<int> graph_distance_tbb(const std::vector<node> &nodes, int start)
{
	std::vector<int> distance(nodes.size(), INT_MAX); // all nodes start at infinite distance
	
	// a list of (id,dist)
	std::deque<std::pair<int,int> > *todo;
	todo->push_back(std::make_pair(start,0));
	
	// parallel do loop
	parallel_do(todo->begin(), todo->end(), ParDo(&distance, nodes, todo));
	
	return distance;
}

#endif
