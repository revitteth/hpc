//#include "fft_tbb.hpp"
//#include <vector>
//#include <stdio.h>
//#include <iostream>
//
//using namespace std;
//
//int main(int argc, char *argv[])
//{
//	srand(0);
//	
//	if(argc<2){
//		fprintf(stderr, "Specify log2 of transform size.");
//		cin.get(); // let the error stay visible in the terminal!
//		exit(1);
//	}
//	
//	int log2n=atoi(argv[1]);
//	int n=1<<log2n;
//	
//	vector<complex<double> > in(n, 0.0), out(n);
//	for(int j=0;j<n;j++){
//		in[j]=complex<double>(rand()/(double)(RAND_MAX) - 0.5, rand()/(double)(RAND_MAX) - 0.5);
//	}
//	
//
//	fft(n, &in[0], &out[0]);
//	
//	for(int j=0;j<n;j++){
//		fprintf(stdout, "%.16lg, %.16lg, %.16lg, %.16lg\n", real(in[j]), imag(in[j]), real(out[j]), imag(out[j]));
//	}
//
//	cin.get();
//	
//	/* To test this, you can try loading the output into matlab. Load
//		the output as a four column matrix x. Then the input is:
//			in=x(:,1)+x(:,2)*i
//		and the output is:
//			out=x(:,3)+x(:,4)*i
//		You can then do fft(in) to check what matlab thinks it should be.
//		Note that this fft and matlab fft produce outputs in different
//		orders. How could you "sort" (hint-hing) that out in matlab so that
//		you can check you have the same spectrum.
//	
//		You can also try putting in various types of sine wave, and checking
//		whether you get the right frequency peaks in the output. You're
//		the signal processing experts, I assume you know what a fourier
//		transform does.
//	
//		Note that this not a very accurate fft, so you should expect
//		the error to grow as the transform size grows.
//	*/
//	
//	return 0;
//}
