#ifndef common_hpp
#define common_hpp

#include <vector>
#include <deque>
#include <utility>

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct node
{
	int id;
	std::vector<int> edges;
};

// This is just a simple platform independent rng, returns an integer in [0,maxn]
unsigned rng(unsigned &seed, unsigned maxn)
{
	seed=(seed*1664525UL*+1013904223UL)&0xFFFFFFFF;
	double u=ldexp((double)seed, -32);	// Convert to [0,1)
	return (unsigned)floor(u*(maxn+1));
}

std::vector<node> build_graph(int n)
{
	std::vector<node> res(n);
	for(int i=0;i<n;i++){
		res[i].id=i;
		res[i].edges.reserve(15);
	}
	
	unsigned seed=1;
	
	for(int i=0;i<10*n;i++){
		int a=rng(seed,n-1);
		int b=rng(seed,n-1);
		
		res[a].edges.push_back(b);
	}
	return res;
}

void dump_graph(const std::vector<node> &nodes)
{
	for(unsigned i=0;i<nodes.size();i++){
		fprintf(stdout, "%d -> ", i);
		for(unsigned j=0;j<nodes[i].edges.size();j++){
			fprintf(stdout, " %d", nodes[i].edges[j]);
		}
		fprintf(stdout, "\n");
	}
}

#endif