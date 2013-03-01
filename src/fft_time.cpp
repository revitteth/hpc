#include "fft.hpp"
#include "fft_tbb.hpp"
#include "fft_opt.hpp"
#include "fft_seq.hpp"
#include "cores.hpp"
#include <vector>
#include <stdio.h>
#include <iostream>

int CoresInformation::coresNo;

int main(int argc, char *argv[])
{
	srand(0);
	
	if(argc<3){
		fprintf(stderr, "Specify log2 of transform size and number of available cores (>1)");
		// hold terminal (windows)
		cin.get();
		exit(1);
	}
	
	int log2n=atoi(argv[1]);
	unsigned int n=1<<log2n;
	int count = 1;
	if(argv[3])
	{
		count = atoi(argv[3]);
	}

	CoresInformation::setCores(atoi(argv[2]));
	
	vector<complex<double> > in(n, 0.0), out(n), out_tbb(n), out_opt(n), out_seq(n);
	for(unsigned j=0;j<n;j++){
		in[j]=complex<double>(rand()/(double)(RAND_MAX) - 0.5, rand()/(double)(RAND_MAX) - 0.5);
	}

	double time_fft(0), time_tbb(0), time_opt(0), time_seq(0);
	int count_fft, count_tbb, count_opt, count_seq;
	count_fft = count_tbb = count_opt = count_seq = count;

	// original
	for(int i = 0; i < count; i++)
	{
		tick_count start_fft = tick_count::now();
		fft(n, &in[0], &out[0]);
		tick_count end_fft = tick_count::now();
		time_fft += (end_fft-start_fft).seconds();
	}
	cout << time_fft/count << endl;

	// tbb enabled
	for(int i = 0; i < count; i++)
	{
		tick_count start_tbb = tick_count::now();
		fft_tbb(n, &in[0], &out_tbb[0]);
		tick_count end_tbb = tick_count::now();
		time_tbb += (end_tbb-start_tbb).seconds();
	}
	cout << time_tbb/count << endl;

	// tbb optimised
	for(int i = 0; i < count; i++)
	{
		tick_count start_opt = tick_count::now();
		fft_opt(n, &in[0], &out_opt[0]);
		tick_count end_opt = tick_count::now();
		time_opt += (end_opt-start_opt).seconds();
	}
	cout << time_opt/count << endl;
	
	// sequential
	for(int i = 0; i < count; i++)
	{
		tick_count start_seq = tick_count::now();
		fft_seq(n, &in[0], &out_seq[0]);
		tick_count end_seq = tick_count::now();
		time_seq += (end_seq-start_seq).seconds();
	}
	cout << time_seq/count << endl;

	// check outputs were all the same as the original code
	for(unsigned j=0;j<n;j++){
		if (real(out[j]) != real(out_tbb[j]) || abs(imag(out[j]) - imag(out_tbb[j])) > 0.01)
			cout << "error in tbb code" << endl;
		if (real(out[j]) != real(out_opt[j]) || abs(imag(out[j]) - imag(out_opt[j])) > 0.01)
			cout << "error in opt code" << endl;
	}
	
	/* To test this, you can try loading the output into matlab. Load
		the output as a four column matrix x. Then the input is:
			in=x(:,1)+x(:,2)*i
		and the output is:
			out=x(:,3)+x(:,4)*i
		You can then do fft(in) to check what matlab thinks it should be.
		Note that this fft and matlab fft produce outputs in different
		orders. How could you "sort" (hint-hing) that out in matlab so that
		you can check you have the same spectrum.
	
		You can also try putting in various types of sine wave, and checking
		whether you get the right frequency peaks in the output. You're
		the signal processing experts, I assume you know what a fourier
		transform does.
	
		Note that this not a very accurate fft, so you should expect
		the error to grow as the transform size grows.
	*/
	
	return 0;
}
