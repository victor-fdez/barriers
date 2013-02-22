
#include <iostream>
#include <mpi.h>
#include "DisseminationBarrier.h"
#include "MCSBarrier.h"

using namespace std;

int main(int argc, char** argv)
{
	MPI::Init(argc, argv);
	///MCSBarrier mcsBarrier(0);
	///mcsBarrier.wait();
	///DisseminationBarrier disseminationBarrier(0);	
	///if(MPI::COMM_WORLD.Get_rank() != 0)
	///disseminationBarrier.wait();
	///MPI::Finalize();
	return 0;
}
