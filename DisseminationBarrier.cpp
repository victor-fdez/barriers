#include <cmath>
#include <mpi.h>
#include "DisseminationBarrier.h"
using namespace std;

DisseminationBarrier::DisseminationBarrier(int numThreads) : Barrier(MPI::COMM_WORLD.Get_size())
{
	int i;
	this->vip = MPI::COMM_WORLD.Get_rank();
	this->logNumberProcesses = (int)(log((double)this->numThreads)/log((double)2.0));
	this->inBuffers = new int[this->logNumberProcesses];	
	this->outBuffers = new int[this->logNumberProcesses];	
	for(i = 0; i < this->logNumberProcesses; i++){
		this->inBuffers[i] = 0;
		this->outBuffers[i] = 0;
	}
}
int DisseminationBarrier::wait()
{
	int k;
	/*go from 0 to log(P-1)*/
	cout << " " << this->vip <<" : arrived at barrier " << this->logNumberProcesses << "\n";
	for(k = 0; k <= this->logNumberProcesses; k++)
	{
		int* in = &this->inBuffers[k];
		int* out = &this->outBuffers[k];
		int offset = 1 << k;
		int inProcess = (this->vip + this->numThreads - offset) % this->numThreads; 
		int outProcess = (this->vip + offset) % this->numThreads;
		/*send processor vip + 2^k mod p message*/
		///cout << " " << this->vip <<" : sending to " << outProcess << "\n";
		MPI::Request requestObject = MPI::COMM_WORLD.Isend(out, 1, MPI::INT, outProcess, 0);
		/*recieve from processor vip + p - 2^k mod p message*/
		MPI::COMM_WORLD.Recv(in, 1, MPI::INT, inProcess, 0);
		///cout << " " << this->vip <<" : recieving from " << inProcess << "\n";
	}
	cout << " " << this->vip <<" : left at barrier\n";
	return 0;
}
