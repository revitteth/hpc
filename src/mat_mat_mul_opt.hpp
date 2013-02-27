#ifndef mat_mat_mul_opt_hpp
#define mat_mat_mul_opt_hpp

#include "mat_t.hpp"
#include "tbb/tbb.h"
#include "cores.hpp"

using namespace tbb;

struct ParallelInnerLoop
{
	private : 
		mat_t *dst, *right;
		unsigned row;

	public :
			ParallelInnerLoop(mat_t *dst, mat_t *right, unsigned row) : dst(dst), right(right), row(row) {}

			void operator() (const blocked_range<size_t>& r) const
			{
				for (size_t col = r.begin(); col != r.end(); ++col)
				{
					dst->at(row,col) += right->at(row, col);
				}
			}

};

struct ParallelOuterLoop
{
	private :
		mat_t *dst, *right;

	public :

		ParallelOuterLoop(mat_t *dst, mat_t *right) : dst(dst), right(right) {}

		void operator() (const blocked_range<size_t>& r) const
		{
			for(size_t row = r.begin(); row != r.end(); ++row)
			{
				parallel_for(blocked_range<size_t>(0, dst->cols), ParallelInnerLoop(dst, right, row));
			}
		}
};


class MatMatMulOpt : public task 
{
	private : 
		mat_t dst, a, b;

	public : 

		MatMatMulOpt(mat_t dst, const mat_t a, const mat_t b) : dst(dst), a(a), b(b) {} 

		task* execute(void)
		{
			if((dst.rows<=128) || (dst.cols<=128))
			{
				// make parallel if bigger than certain value? Is it even worth it in terms of a speed up?
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
				task_list tasks;
				set_ref_count(9); // number of tasks on list + root task

				local_mat_t right(dst.rows, dst.cols);
		
				tasks.push_back(*new (task::allocate_child()) MatMatMulOpt(dst.quad(0,0), a.quad(0,0), b.quad(0,0)));
				tasks.push_back(*new (task::allocate_child()) MatMatMulOpt(dst.quad(0,1), a.quad(0,0), b.quad(0,1)));
				tasks.push_back(*new (task::allocate_child()) MatMatMulOpt(dst.quad(1,0), a.quad(1,0), b.quad(0,0)));
				tasks.push_back(*new (task::allocate_child()) MatMatMulOpt(dst.quad(1,1), a.quad(1,0), b.quad(0,1)));
		
				tasks.push_back(*new (task::allocate_child()) MatMatMulOpt(right.quad(0,0), a.quad(0,1), b.quad(1,0)));
				tasks.push_back(*new (task::allocate_child()) MatMatMulOpt(right.quad(0,1), a.quad(0,1), b.quad(1,1)));
				tasks.push_back(*new (task::allocate_child()) MatMatMulOpt(right.quad(1,0), a.quad(1,1), b.quad(1,0)));
				tasks.push_back(*new (task::allocate_child()) MatMatMulOpt(right.quad(1,1), a.quad(1,1), b.quad(1,1)));
				
				spawn_and_wait_for_all(tasks);

				parallel_for(blocked_range<size_t>(0, dst.rows), ParallelOuterLoop(&dst, &right));
			}
			return NULL;
		}
};

void mat_mat_mul_opt(mat_t dst, mat_t a, mat_t b)
{
	task_scheduler_init init(CoresInformation::getCores());
	MatMatMulOpt &taskRoot = *new (task::allocate_root()) MatMatMulOpt(dst,a,b);
	task::spawn_root_and_wait(taskRoot);
}

#endif