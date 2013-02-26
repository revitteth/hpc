#include "mat_mat_mul.hpp"
#include "mat_mat_mul_tbb.hpp"
#include "mat_mat_mul_opt.hpp"

#include "tbb/tick_count.h"

using namespace tbb;
using namespace std;

int main(int argc, char *argv[])
{
	srand(0);
	
	int n=8;
	if(argc>1)
		n=atoi(argv[1]);
		
	local_mat_t A(n,n), B(n,n);
	local_mat_t R_orig(n,n), R_tbb(n,n), R_opt(n,n);
	
	A.randomise();
	A.dump(std::cout);
	
	B.randomise();
	B.dump(std::cout);
	
	// original code
	tick_count start_orig = tick_count::now();
	mat_mat_mul(R_orig, A, B);
	tick_count end_orig = tick_count::now();
	double original_time = (end_orig - start_orig).seconds();

	// tbb enabled
	tick_count start_tbb = tick_count::now();
	mat_mat_mul_tbb(R_tbb, A, B);
	tick_count end_tbb = tick_count::now();
	double tbb_time = (end_tbb - start_tbb).seconds();

	// tbb optimised
	tick_count start_opt = tick_count::now();
	mat_mat_mul_opt(R_opt, A, B);
	tick_count end_opt = tick_count::now();
	double opt_time = (end_opt - start_opt).seconds();

	// Check for errors in output
	for (unsigned row = 0; row < R_orig.rows; row++)
	{
		for (unsigned col = 0; col < R_orig.cols; col++)
		{
			if (abs(R_tbb.at(row, col) - R_orig.at(row, col)) > 0.01)
			{
				cout << "error in basic tbb code" << endl;
			}
			if (abs(R_opt.at(row, col) - R_orig.at(row, col)) > 0.01)
			{
				cout << "error in optimised tbb code" << endl;
			}
		}

	}

	//R.dump(cout);
	
	cout << original_time << " seconds" << endl;
	cout << tbb_time << " seconds" << endl;
	cout << opt_time << " seconds" << endl;


	// hold console open
	cin.get();

	return 0;
}
