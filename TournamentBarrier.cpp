#include <iostream>
#include <cmath>
#include <omp.h>
#include <assert.h>
#include "Barrier.h"
#include "TournamentBarrier.h"

using namespace std;

///TODO: setup rounds array such that there is not contention due to 
///false sharing
TournamentBarrier::TournamentBarrier(int numThreads) : Barrier(numThreads)
{
	int i, k;
	///should check first that numThreads is 2^k for some k
	this->numberRounds = (int)ceil((log((double)numThreads)/log((double)2.0)));
	this->rounds = new round_t[numThreads*(this->numberRounds+1)];
	this->sense = new sense_t[numThreads]; 
	cout << "number of rounds " << numberRounds << "\n";
	///cout << "number of thread " << this->numThreads << "\n";
	for(i = 0; i < this->numThreads; i++)
	{
		this->sense[i].value = 1;
	}
	///intialize all other roles
	for(k = 0; k <= this->numberRounds; k++)
	{
		for(i = 0; i < this->numThreads; i++)
		{
			this->rounds[round(i, k)].role = UNUSED;
			this->rounds[round(i, k)].flag = 0;
			this->rounds[round(i, k)].opponent = NULL;
		}
	}
	///setup WINNERS and LOSERS
	for(k = 1; k <= this->numberRounds; k++)
	{
		int increment = 1 << k;
		int offset = 1 << k - 1;
		for(i = 0; i < this->numThreads; i += increment)
		{
			round_t* winner = this->rounds + round(i, k);
			round_t* loser = this->rounds + round(i+offset, k);
			winner->role = WINNER;
			winner->opponent = &loser->flag;
			loser->role = LOSER;
			loser->opponent = &winner->flag;
		}
	}
	///setup CHAMPIONS
	this->rounds[round(0, this->numberRounds)].role = CHAMPION;
	///setup DROPOUT
	for(i = 0; i < this->numThreads; i++)
		this->rounds[round(i, 0)].role = DROPOUT;
}

int TournamentBarrier::wait()
{
	int round = 1;
	int vip = omp_get_thread_num();
	int localSense = this->sense[vip].value;
	///arrival of threads
	while(1)
	{
		switch(this->rounds[round(vip, round)].role)
		{
			case CHAMPION :
				//wait while opponent sets my flag
///				#pragma omp critical(tournamentMsg)
///				{
///					cout << "round " << round << ": thread " << vip << " is champion\n";
///				}
				while(this->rounds[round(vip, round)].flag != localSense);
				*(this->rounds[round(vip, round)].opponent) = localSense;
				#pragma omp flush
				goto tournamentBarrierWakeup;
			case WINNER :
///				#pragma omp critical(tournamentMsg)
///				{
///					cout << "round " << round << ": thread " << vip << " is winner\n";
///				}
				while(this->rounds[round(vip, round)].flag != localSense);
				break;
			case LOSER :
///				#pragma omp critical(tournamentMsg)
///				{
///					cout << "round " << round << ": thread " << vip << " loses\n";
///				}
				*(this->rounds[round(vip, round)].opponent) = localSense;
				#pragma omp flush
				while(this->rounds[round(vip, round)].flag != localSense);
				goto tournamentBarrierWakeup;
			default :
				assert(0);
				///something is wrong
				break;
		}		
///		#pragma omp critical(tournamentMsg)
///		{
///			cout << "round " << round << ": thread " << vip << " next round\n";
///		}
		round++;
	}
	///wake up of threads
tournamentBarrierWakeup:
	while(1)
	{
		round--;
		switch(this->rounds[round(vip, round)].role)
		{
			case CHAMPION :
				assert(0);
				break;
			case WINNER :
				*(this->rounds[round(vip, round)].opponent) = localSense;
				#pragma omp flush
				break;
			case LOSER :
				assert(0);
				break;
			case DROPOUT :
///				#pragma omp critical(tournamentMsg)
///				{
///					cout << "thread " << vip << " finished\n";
///				}
				goto tournamentBarrierFinish;
			default :
				break;
		}		

	}
tournamentBarrierFinish:
	#pragma omp critical(sense)
	{
		this->sense[vip].value = !localSense;
	}
	return 0;
}

void TournamentBarrier::printRounds()
{
	int i, j;
	for(i = 0; i < this->numThreads; i++)
	{
		cout << i << " [";
		for(j = 0; j <= this->numberRounds; j++)
		{
			int role = this->rounds[round(i,j)].role;
			int flag = this->rounds[round(i,j)].flag;
			if(role == WINNER)
				cout << "W";
			else if(role == LOSER)
				cout << "L";
			else if(role == CHAMPION)
				cout << "C";
			else if(role == DROPOUT)
				cout << "D";
			else
				cout << "U";
			cout << flag;
			//backspacing
			if(j + 1 > this->numberRounds)
				cout << "]";
			else
				cout << ", ";
		}
		cout << "\n";
	}
}
TournamentBarrier::~TournamentBarrier()
{
	delete[] this->rounds;
	delete[] this->sense;
}
