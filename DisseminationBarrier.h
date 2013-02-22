#ifndef __DISSEMINATION_BARRIER_H__
#define __DISSEMINATION_BARRIER_H__
#include "Barrier.h"

class DisseminationBarrier : public Barrier
{
	public:
		DisseminationBarrier(int numThreads);
		int wait();
	private:
		int vip;
		int logNumberProcesses;
		int *outBuffers;
		int *inBuffers;
};



#endif
