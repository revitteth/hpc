#include "mat_mat_mul.hpp"
#include "mat_mat_mul_tbb.hpp"
#include "mat_mat_mul_opt.hpp"
#include "mat_mat_mul_seq.hpp"
#include "cores.hpp"

#include "tbb/tick_count.h"

using namespace tbb;
using namespace std;

int CoresInformation::coresNo;

int main(int argc, char *argv[])
{
	srand(0);
	
	int n=8;
	if(argc>2)
	{
		n=atoi(argv[1]);
		CoresInformation::setCores(atoi(argv[2]));
	}
	else
	{
		cout << "please enter n (size of matrix) and number of cpus" << endl;
		cin.get();
		return 1;
	}

		
	local_mat_t A(n,n), B(n,n);
	local_mat_t R_orig(n,n), R_tbb(n,n), R_opt(n,n), R_seq(n,n);
	
	A.randomise();
	//A.dump(std::cout);
	
	B.randomise();
	//B.dump(std::cout);
	

	double time_orig(0), time_tbb(0), time_opt(0), time_seq(0);
	int count = 1;
	int count_orig, count_tbb, count_opt, count_seq;
	count_orig = count_tbb = count_opt = count_seq = count;

	// original
	for(int i = 0; i < count; i++)
	{
		tick_count start_orig = tick_count::now();
		mat_mat_mul(R_orig, A, B);
		tick_count end_orig = tick_count::now();
		time_orig += (end_orig-start_orig).seconds();
	}
	cout << time_orig/count << endl;

	// tbb enabled
	for(int i = 0; i < count; i++)
	{
		tick_count start_tbb = tick_count::now();
		mat_mat_mul_tbb(R_tbb, A, B);
		tick_count end_tbb = tick_count::now();
		time_tbb += (end_tbb-start_tbb).seconds();
	}	
	cout << time_tbb/count << endl;

	// tbb optimised
	for(int i = 0; i < count; i++)
	{
		tick_count start_opt = tick_count::now();
		mat_mat_mul_opt(R_opt, A, B);
		tick_count end_opt = tick_count::now();
		time_opt += (end_opt-start_opt).seconds();
	}
	cout << time_opt/count << endl;

	// sequential
	for(int i = 0; i < count; i++)
	{
		tick_count start_seq = tick_count::now();
		mat_mat_mul_seq(R_seq, A, B);
		tick_count end_seq = tick_count::now();
		time_seq += (end_seq-start_seq).seconds();
	}
	cout << time_seq/count << endl;

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
			if (abs(R_seq.at(row, col) - R_orig.at(row, col)) > 0.01)
			{
				cout << "error in sequential code" << endl;
			}
		}
	}

	//R.dump(cout);

	// hold console open
	cin.get();

	return 0;
}
