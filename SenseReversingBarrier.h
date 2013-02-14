
#ifndef __SENSE_REVERSING_BARRER_H__
#define __SENSE_REVERSING_BARRER_H__
#include "Barrier.h"

using namespace std;

class SenseReversingBarrier : public Barrier
{
	public:
		///initializes the barrier
		SenseReversingBarrier(int numThreads);
		///must implement wait() because virtual method in barrier
		int wait();
	private:
		int count;
		int sense;
		int numThreads;
};

#endif
