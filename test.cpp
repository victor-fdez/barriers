#include <iostream>
#include <omp.h>
#include "SenseReversingBarrier.h"
#include "TournamentBarrier.h"

using namespace std;

int main(int argc, char** argv)
{
	///SenseReversingBarrier senseBarrier(4);
	TournamentBarrier tournamentBarrier(4);	
	///tournamentBarrier.printRounds();
	#pragma omp parallel num_threads(4)
	{
		tournamentBarrier.wait();
	}
	///tournamentBarrier.~TournamentBarrier();
	return 0;
}
