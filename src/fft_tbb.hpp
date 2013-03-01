#ifndef fft_tbb_hpp
#define fft_tbb_hpp

#include <complex>
#include <tbb/tbb.h>
#include "cores.hpp"

using namespace tbb;

/* Does a recursive FFT
	n = Number of elements (must be a power of two)
	wn =  Complex root of unity of order n
	(pIn,sIn) = Base and stride of input data
	(pOut,Sout) = Base and stride of output data
*/
class FftTbb : public task
{
	private :
		int n, sIn, sOut;
		std::complex<double> wn;
		const std::complex<double> *pIn;
		std::complex<double> *pOut;

	public :

		FftTbb(	int n, std::complex<double> wn,
				const std::complex<double> *pIn, int sIn,
				std::complex<double> *pOut, int sOut)
				: n(n), wn(wn), pIn(pIn), sIn(sIn), pOut(pOut), sOut(sOut) {}

		task* execute(void)
		{
			if (n == 1){
				pOut[0] = pIn[0];
			}else if (n == 2){
				pOut[0] = pIn[0]+pIn[sIn];
				pOut[sOut] = pIn[0]-pIn[sIn];
			}else{
				unsigned m = n/2;

				task_list tasks;
				set_ref_count(3);

				// create tasks
				tasks.push_back(*new (task::allocate_child()) FftTbb(m,wn*wn,pIn,2*sIn,pOut,sOut));
				tasks.push_back(*new (task::allocate_child()) FftTbb(m,wn*wn,pIn+sIn,2*sIn,pOut+sOut*m,sOut));

				spawn_and_wait_for_all(tasks);
		 
				std::complex<double> w=std::complex<double>(1.0, 0.0);

				for (unsigned j=0;j<m;j++){
				  std::complex<double> t1 = w*pOut[m+j];
				  std::complex<double> t2 = pOut[j]-t1;
				  pOut[j] = pOut[j]+t1;                 /*  pOut[j] = pOut[j] + w^i pOut[m+j] */
				  pOut[j+m] = t2;                       /*  pOut[j] = pOut[j] - w^i pOut[m+j] */
				  w = w*wn;
				}
			}
			return NULL;
		}
};
	
void fft_tbb(int n, const std::complex<double> *pIn, std::complex<double> *pOut)
{
	const double pi2=6.283185307179586476925286766559;
	double angle = pi2/n;
	std::complex<double> wn(cos(angle), sin(angle));
	task_scheduler_init init(CoresInformation::getCores());
	FftTbb &taskRoot = *new (task::allocate_root()) FftTbb(n, wn, pIn, 1, pOut, 1);
	// wait for root task to finish
	task::spawn_root_and_wait(taskRoot);
}

#endif