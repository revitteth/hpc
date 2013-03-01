#ifndef fft_seq_hpp
#define fft_seq_hpp

#include <complex>
#include <tbb/tbb.h>

using namespace tbb;
using namespace std;

/* Does a recursive FFT
	n = Number of elements (must be a power of two)
	wn =  Complex root of unity of order n
	(pIn,sIn) = Base and stride of input data
	(pOut,Sout) = Base and stride of output data
*/
class FftSeq
{
	private :
		int n, sIn, sOut;
		complex<double> wn;
		const complex<double> *pIn;
		complex<double> *pOut;

	public :

		FftSeq(	int n, complex<double> wn,
				const complex<double> *pIn, int sIn,
				complex<double> *pOut, int sOut)
				: n(n), wn(wn), pIn(pIn), sIn(sIn), pOut(pOut), sOut(sOut) {}

		void execute(void)
		{
			if (n == 1){
				pOut[0] = pIn[0];
			}else if (n == 2){
				pOut[0] = pIn[0]+pIn[sIn];
				pOut[sOut] = pIn[0]-pIn[sIn];
			}else{
				unsigned m = n/2;

				FftSeq(m,wn*wn,pIn,2*sIn,pOut,sOut).execute();
				FftSeq(m,wn*wn,pIn+sIn,2*sIn,pOut+sOut*m,sOut).execute();

				complex<double> w = complex<double>(1.0, 0.0);

				for (unsigned j=0;j<m;j++){
					std::complex<double> t1 = w*pOut[m+j];
					std::complex<double> t2 = pOut[j]-t1;
					pOut[j] = pOut[j]+t1;                 /*  pOut[j] = pOut[j] + w^i pOut[m+j] */
					pOut[j+m] = t2;                       /*  pOut[j] = pOut[j] - w^i pOut[m+j] */
					w = w*wn;
				}
			}
		}
};
	
void fft_seq(int n, const complex<double> *pIn, complex<double> *pOut)
{
	const double pi2=6.283185307179586476925286766559;
	double angle = pi2/n;
	complex<double> wn(cos(angle), sin(angle));
	FftSeq(n, wn, pIn, 1, pOut, 1).execute();
}

#endif