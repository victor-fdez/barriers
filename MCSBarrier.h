#ifndef __MSC_BARRIER_H__
#define __MSC_BARRIER_H__
#include "Barrier.h"

class MCSBarrier : public Barrier
{
	public:
		MCSBarrier(int numThreads);
		int wait();
	private:
		void resetChildNotReady();
		void print();
		int vip;
		char childNotReady[4];

};

#endif
