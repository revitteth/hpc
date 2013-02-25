#include "mat_mat_mul_tbb.hpp"
#include "tbb/tick_count.h"

using namespace tbb;

int main(int argc, char *argv[])
{
	srand(0);
	
	int n=8;
	if(argc>1)
		n=atoi(argv[1]);
		
	local_mat_t A(n,n), B(n,n);
	local_mat_t R(n,n);
	
	A.randomise();
	//A.dump(std::cout);
	
	B.randomise();
	//B.dump(std::cout);
	
	// Collect start time
	tick_count start = tick_count::now();

	mat_mat_mul(R, A, B);

	// Collect end time
	tick_count end = tick_count::now();

	//R.dump(std::cout);
	
	std::cout << (end - start).seconds();
	std::cout << " seconds";

	// hold console open
	std::cin.get();

	return 0;
}
