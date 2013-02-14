
#ifndef __BARRIER_H__
#define __BARRIER_H__

using namespace std;

class Barrier
{
	public:
		Barrier(int numThreads) : numThreads(numThreads) {}
		virtual int wait() = 0;
	protected:
		int numThreads;
};

#endif
