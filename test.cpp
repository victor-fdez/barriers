#include <iostream>
#include <omp.h>
#include "SenseReversingBarrier.h"
#include "TournamentBarrier.h"

using namespace std;

int main(int argc, char** argv)
{
	SenseReversingBarrier senseBarrier(4);
	TournamentBarrier tournamentBarrier(16);	
	tournamentBarrier.printRounds();
///	#pragma omp parallel num_threads(4)
///	{
///		barrier.wait();
///	}
	return 0;
}
