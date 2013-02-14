#ifndef __TOURNAMENT_BARRIER_H__
#define __TOURNAMENT_BARRIER_H__
#include "Barrier.h"
///macro to round in array
///round
///@param i index of thread
///@param j index of round
#define round(i, j) 	(((i)*(this->numberRounds+1))+(j))

///player definitions
#define CHAMPION 	0
#define WINNER		1
#define LOSER		2	
#define DROPOUT		3
#define UNUSED		4

using namespace std;

typedef struct
{
	int role;
	int* opponent;
	int flag;	
	///may need to add padding depending on performance, to reduce contention
} round_t;
typedef struct
{
	int value;
	///may need to add padding depending on performance, to reduce contention
} sense_t;

class TournamentBarrier : public Barrier
{
	public:
		TournamentBarrier(int numThreads);
		int wait();
		void printRounds();
		~TournamentBarrier();
	private:
		int numberRounds;
		round_t* rounds;
		sense_t* sense;
};

#endif
