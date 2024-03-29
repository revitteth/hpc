#ifndef mat_mat_mul_opt_hpp
#define mat_mat_mul_opt_hpp

#include "mat_t.hpp"
#include "tbb/tbb.h"
#include "cores.hpp"

using namespace tbb;

// Parallelisation of the inner first loop
struct FirstParallelInnerLoop
{
	private : 
		mat_t *dst, *a, *b;
		int row;

	public :
			FirstParallelInnerLoop(mat_t *dst, mat_t *a, mat_t *b, int row) : dst(dst), a(a), b(b), row(row) {}

			void operator() (const blocked_range<size_t>& r) const
			{
				for (size_t col = r.begin(); col != r.end(); ++col)
				{
				double acc=0.0;
				for(unsigned i=0;i<(*a).cols;i++)
				{
					acc += (*a).at(row,i) * (*b).at(i,col);
				}
				(*dst).at(row,col) = acc;
				}
			}

};

// Parallelisation of the outer first loop
struct FirstParallelOuterLoop
{
	private :
		mat_t *dst, *a, *b;

	public :

		FirstParallelOuterLoop(mat_t *dst, mat_t *a, mat_t *b) : dst(dst), a(a), b(b) {}

		void operator() (const blocked_range<size_t>& r) const
		{
			for(size_t row = r.begin(); row != r.end(); ++row)
			{
				parallel_for(blocked_range<size_t>(0, dst->cols), FirstParallelInnerLoop(dst, a, b, row));
			}
		}
};

// Parallelisation of the last inner loop
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

// Parallelisation of the last outer loop
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
				// use structs. above to perform parallel for loop
				parallel_for(blocked_range<size_t>(0, dst.rows), FirstParallelOuterLoop(&dst, &a, &b));
			}
			else
			{
				task_list tasks;
				set_ref_count(9); // number of tasks on list + root task

				local_mat_t right(dst.rows, dst.cols);
				
				// create tasks
				tasks.push_back(*new (task::allocate_child()) MatMatMulOpt(dst.quad(0,0), a.quad(0,0), b.quad(0,0)));
				tasks.push_back(*new (task::allocate_child()) MatMatMulOpt(dst.quad(0,1), a.quad(0,0), b.quad(0,1)));
				tasks.push_back(*new (task::allocate_child()) MatMatMulOpt(dst.quad(1,0), a.quad(1,0), b.quad(0,0)));
				tasks.push_back(*new (task::allocate_child()) MatMatMulOpt(dst.quad(1,1), a.quad(1,0), b.quad(0,1)));
		
				tasks.push_back(*new (task::allocate_child()) MatMatMulOpt(right.quad(0,0), a.quad(0,1), b.quad(1,0)));
				tasks.push_back(*new (task::allocate_child()) MatMatMulOpt(right.quad(0,1), a.quad(0,1), b.quad(1,1)));
				tasks.push_back(*new (task::allocate_child()) MatMatMulOpt(right.quad(1,0), a.quad(1,1), b.quad(1,0)));
				tasks.push_back(*new (task::allocate_child()) MatMatMulOpt(right.quad(1,1), a.quad(1,1), b.quad(1,1)));
				
				spawn_and_wait_for_all(tasks);

				// use structs above to perform parallel for loop
				parallel_for(blocked_range<size_t>(0, dst.rows), ParallelOuterLoop(&dst, &right));
			}
			return NULL;
		}
};

void mat_mat_mul_opt(mat_t dst, mat_t a, mat_t b)
{
	task_scheduler_init init(CoresInformation::getCores());
	MatMatMulOpt &taskRoot = *new (task::allocate_root()) MatMatMulOpt(dst,a,b);
	// wait for root task
	task::spawn_root_and_wait(taskRoot);
}

#endif