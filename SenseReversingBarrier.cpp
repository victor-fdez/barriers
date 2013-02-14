#include <iostream>
#include <omp.h>
#include "SenseReversingBarrier.h"

SenseReversingBarrier::SenseReversingBarrier(int numThreads) : Barrier(numThreads), count(numThreads), sense(0)
{
}

int SenseReversingBarrier::wait()
{
	int threadId = omp_get_thread_num();
	int localSense = !(this->sense);	
	int localCount = 0; 
	///fetch & decrement atomically
	#pragma omp critical(fetch_and_dec_sense)
	{
		localCount = (this->count--);
	}
///	#pragma omp critical(print_msg)
///	{
///		cout << "thread " << threadId << " count " << localCount << ", localSense " << localSense << "\n";
///	}
	if(localCount == 1)
	{
		///cout << "thread " << threadId << " releases!\n";
		///last processor toggles flag
		this->count = this->numThreads;
		this->sense = localSense;
		#pragma omp flush
	}
	else
	{
		#pragma omp flush
		while(this->sense != localSense)
		{
			#pragma omp flush
		}
	}
///	#pragma omp critical(print_msg)
///	{
///		cout << "thread " << threadId << " released\n";
///	}
	return 0;
}
