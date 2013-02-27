#ifndef fft_opt_hpp
#define fft_opt_hpp

#include <complex>
#include <tbb/tbb.h>

using namespace tbb;
using namespace std;

struct ParallelLoop
{
	private : 
		complex<double> *w, *wn, *pOut;
		unsigned m;

	public :
			ParallelLoop(unsigned m, complex<double> *wn, complex<double> *w, complex<double> *pOut) 
				: m(m), wn(wn), w(w), pOut(pOut) {}

			void operator() (const blocked_range<size_t>& r) const
			{
				for (size_t j = r.begin(); j != r.end(); ++j)
				{
					complex<double> t1 = (*w)*pOut[m+j];
					complex<double> t2 = pOut[j]-t1;
					pOut[j] = pOut[j]+t1;                 /*  pOut[j] = pOut[j] + w^i pOut[m+j] */
					pOut[j+m] = t2;                       /*  pOut[j] = pOut[j] - w^i pOut[m+j] */
					(*w) = (*w)*(*wn);
				}
			}

};

/* Does a recursive FFT
	n = Number of elements (must be a power of two)
	wn =  Complex root of unity of order n
	(pIn,sIn) = Base and stride of input data
	(pOut,Sout) = Base and stride of output data
*/
class FftOpt : public task
{
	private :
		int n, sIn, sOut;
		complex<double> wn;
		const complex<double> *pIn;
		complex<double> *pOut;

	public :

		FftOpt(	int n, complex<double> wn,
				const complex<double> *pIn, int sIn,
				complex<double> *pOut, int sOut)
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

				tasks.push_back(*new (task::allocate_child()) FftOpt(m,wn*wn,pIn,2*sIn,pOut,sOut));
				tasks.push_back(*new (task::allocate_child()) FftOpt(m,wn*wn,pIn+sIn,2*sIn,pOut+sOut*m,sOut));

				spawn_and_wait_for_all(tasks);
		 
				complex<double> w = complex<double>(1.0, 0.0);

				//parallel_for(blocked_range<size_t>(0, m), ParallelLoop(m, &wn, &w, pOut));

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
	
void fft_opt(int n, const complex<double> *pIn, complex<double> *pOut)
{
	const double pi2=6.283185307179586476925286766559;
	double angle = pi2/n;
	complex<double> wn(cos(angle), sin(angle));
	FftOpt &taskRoot = *new (task::allocate_root()) FftOpt(n, wn, pIn, 1, pOut, 1);
	task::spawn_root_and_wait(taskRoot);
}

#endif
