#include "fft.hpp"
#include "fft_tbb.hpp"
#include "fft_opt.hpp"
#include "fft_seq.hpp"
#include <vector>
#include <stdio.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
	srand(0);
	
	if(argc<2){
		fprintf(stderr, "Specify log2 of transform size.");
		cin.get(); // let the error stay visible in the terminal!
		exit(1);
	}
	
	int log2n=atoi(argv[1]);
	int n=1<<log2n;
	
	vector<complex<double> > in(n, 0.0), out(n), out_tbb(n), out_opt(n);
	for(int j=0;j<n;j++){
		in[j]=complex<double>(rand()/(double)(RAND_MAX) - 0.5, rand()/(double)(RAND_MAX) - 0.5);
	}

	tick_count start_fft = tick_count::now();
	fft(n, &in[0], &out[0]);
	tick_count end_fft = tick_count::now();

	tick_count start_tbb = tick_count::now();
	fft_tbb(n, &in[0], &out_tbb[0]);
	tick_count end_tbb = tick_count::now();

	tick_count start_opt = tick_count::now();
	fft_opt(n, &in[0], &out_opt[0]);
	tick_count end_opt = tick_count::now();
	
	for(int j=0;j<n;j++){
		//fprintf(stdout, "%.16lg, %.16lg, %.16lg, %.16lg\n", real(in[j]), imag(in[j]), real(out[j]), imag(out[j]));
		if (real(out[j]) != real(out_tbb[j]) || imag(out[j]) != imag(out_tbb[j]))
			cout << "error in tbb code" << endl;
		if (real(out[j]) != real(out_opt[j]) || imag(out[j]) != imag(out_opt[j]))
			cout << "error in opt code" << endl;
	}
	cout << (end_fft-start_fft).seconds()/100000 << endl;
	cout << (end_tbb-start_tbb).seconds()/100000 << endl;
	cout << (end_opt-start_opt).seconds()/100000 << endl;
	cin.get();
	
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
