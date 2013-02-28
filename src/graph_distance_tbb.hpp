#ifndef graph_distance_tbb_hpp
#define graph_distance_tbb_hpp

#include "common.hpp"
#include <tbb/tbb.h>

using namespace tbb;

struct ParDo {
	private :
		std::vector<int> *distance;
		const std::vector<node> *nodes;

	public :
		ParDo(std::vector<int>* distance, const std::vector<node> *nodes) : distance(distance), nodes(nodes) {};

		void operator() (std::pair<int,int> &td, parallel_do_feeder<std::pair<int,int> > &feeder) const
		{
			std::pair<int,int> curr = td;
			
			if((*distance)[curr.first] >= curr.second){
				(*distance)[curr.first]=curr.second;
				for(unsigned i=0;i<(*nodes)[curr.first].edges.size();i++){
					feeder.add(std::make_pair((*nodes)[curr.first].edges[i],curr.second+1));
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
	std::deque<std::pair<int,int> > todo;
	todo.push_back(std::make_pair(start,0));
	
	// parallel do loop
	parallel_do(todo.begin(), todo.end(), ParDo(&distance, &nodes));
	
	return distance;
}

#endif
