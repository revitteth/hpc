#ifndef mat_mat_mul_tbb_opt
#define mat_mat_mul_tbb_opt

#include "mat_t.hpp"
#include "tbb/task.h"
#include "tbb/task_scheduler_init.h"

using namespace tbb;

class MatMatMulOpt : public tbb::task 
{
	private : 
		mat_t dst;
		mat_t a;
		mat_t b;

	public : 
		MatMatMulOpt(mat_t dst, const mat_t a, const mat_t b) : dst(dst), a(a), b(b) {}

		task* execute(void)
		{
			if((dst.rows==1) || (dst.cols==1))
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
				task_list tasks;

				local_mat_t right(dst.rows, dst.cols);
		
				tasks.push_back(*new (task::allocate_child()) MatMatMulOpt(dst.quad(0,0), a.quad(0,0), b.quad(0,0)));
				tasks.push_back(*new (task::allocate_child()) MatMatMulOpt(dst.quad(0,1), a.quad(0,0), b.quad(0,1)));
				tasks.push_back(*new (task::allocate_child()) MatMatMulOpt(dst.quad(1,0), a.quad(1,0), b.quad(0,0)));
				tasks.push_back(*new (task::allocate_child()) MatMatMulOpt(dst.quad(1,1), a.quad(1,0), b.quad(0,1)));
		
				tasks.push_back(*new (task::allocate_child()) MatMatMulOpt(right.quad(0,0), a.quad(0,1), b.quad(1,0)));
				tasks.push_back(*new (task::allocate_child()) MatMatMulOpt(right.quad(0,1), a.quad(0,1), b.quad(1,1)));
				tasks.push_back(*new (task::allocate_child()) MatMatMulOpt(right.quad(1,0), a.quad(1,1), b.quad(1,0)));
				tasks.push_back(*new (task::allocate_child()) MatMatMulOpt(right.quad(1,1), a.quad(1,1), b.quad(1,1)));
				
				set_ref_count(9); // number of tasks on list + root task

				spawn_and_wait_for_all(tasks);

				for(unsigned row=0;row<dst.rows;row++)
				{
					for(unsigned col=0;col<dst.cols;col++)
					{
						dst.at(row,col) += right.at(row,col);
					}
				}
			}
			return NULL;
		}
};

void mat_mat_mul(mat_t dst, mat_t a, mat_t b)
{
	MatMatMulOpt &taskRoot = *new (task::allocate_root()) MatMatMulOpt(dst,a,b);
	task::spawn_root_and_wait(taskRoot);
}

#endif