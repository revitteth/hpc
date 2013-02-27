#ifndef mat_mat_mul_seq_hpp
#define mat_mat_mul_seq_hpp

#include "mat_t.hpp"


class MatMatMulSeq
{
	private : 
		mat_t dst, a, b;

	public : 

		MatMatMulSeq(mat_t dst, const mat_t a, const mat_t b) : dst(dst), a(a), b(b) {} 

		// Task containing functionality
		void execute(void)
		{
			if((dst.rows<=128) || (dst.cols<=128))
			{
				for(unsigned row=0;row<dst.rows;row++)
				{
					for(unsigned col=0;col<dst.cols;col++)
					{
						double acc=0.0;
						for(unsigned i=0;i<a.cols;i++)
						{
							acc += a.at(row,i) * b.at(i,col);
						}
						dst.at(row,col) = acc;
					}
				}
			}
			else
			{

				local_mat_t right(dst.rows, dst.cols);
		
				MatMatMulSeq(dst.quad(0,0), a.quad(0,0), b.quad(0,0)).execute();
				MatMatMulSeq(dst.quad(0,1), a.quad(0,0), b.quad(0,1)).execute();
				MatMatMulSeq(dst.quad(1,0), a.quad(1,0), b.quad(0,0)).execute();
				MatMatMulSeq(dst.quad(1,1), a.quad(1,0), b.quad(0,1)).execute();
		
				MatMatMulSeq(right.quad(0,0), a.quad(0,1), b.quad(1,0)).execute();
				MatMatMulSeq(right.quad(0,1), a.quad(0,1), b.quad(1,1)).execute();
				MatMatMulSeq(right.quad(1,0), a.quad(1,1), b.quad(1,0)).execute();
				MatMatMulSeq(right.quad(1,1), a.quad(1,1), b.quad(1,1)).execute();
				
				for(unsigned row=0;row<dst.rows;row++)
				{
					for(unsigned col=0;col<dst.cols;col++)
					{
						dst.at(row,col) += right.at(row,col);
					}
				}
			}
		}
};

void mat_mat_mul_seq(mat_t dst, mat_t a, mat_t b)
{
	MatMatMulSeq(dst,a,b).execute();
}

#endif