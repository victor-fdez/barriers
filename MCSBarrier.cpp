#include <iostream>
#include <mpi.h>
#include "MCSBarrier.h"

using namespace std;

MCSBarrier::MCSBarrier(int numThreads) : Barrier(MPI::COMM_WORLD.Get_size())
{
	this->vip = MPI::COMM_WORLD.Get_rank();
	resetChildNotReady();
}

int MCSBarrier::wait()
{
	int j;
	int arrivalParent = (this->vip - 1) / 4;
	int wakeParent = (this->vip - 1) / 2;
	int arrivalFirstChild = (4*(this->vip))+1;
	int wakeFirstChild = (2*(this->vip))+1;
	char out = 0;
	///cout << " " << this->vip <<" : arrived at barrier\n";
	//this->print();
	/*wait until all all childs have responded*/
	while(*((int*)this->childNotReady))
	{
		/*collect four values from children*/
		//cout << "collecting children " << this->vip << "\n";
		//cout << *((int*)this->childNotReady) << " : " << this->vip  << "\n";
		for(j = 0; j < 4; j++)
		{
			char in = 1;
			int arrivalChild = arrivalFirstChild + j;
			///MPI::Status& status(;
			if(this->childNotReady[j] == 0 || arrivalChild >= this->numThreads)
				continue;
			/*try to recieve from child*/
			MPI::COMM_WORLD.Recv(&in, 1, MPI::CHAR, arrivalChild, 0);
			///cout << "recieved from child \"" << (int)in << "\" " << arrivalChild << " : " << this->vip << "\n";
			if(in == 0)
			{
				this->childNotReady[j] = in;
				///this->print();
			}
			else
				cout << "this should not happen\n";
		}
	} 	
	///cout << "finished waiting for children : " << this->vip << "\n";
	/*clean for next round*/
	this->resetChildNotReady();
	/*send to parent if this is not the root node*/
	if(this->vip != 0){
		/*send ready to parent node*/
		///cout << "sending to parent " << arrivalParent << " : " << this->vip << "\n";
		MPI::COMM_WORLD.Send(&out, 1, MPI::CHAR, arrivalParent, 0);
	}
	/*root node does not wait for parent*/
	if(this->vip != 0)
	{
		char in;
		/*wait until parent has waken up this node*/
		MPI::COMM_WORLD.Recv(&in, 1, MPI::CHAR, wakeParent, 0);
		///cout << "recieved from parent " << wakeParent << " : " << this->vip << "\n";
	}
	/*wake up first child then second child*/
	if(wakeFirstChild < this->numThreads){
		///cout << "sending to child " << wakeFirstChild << " : " << this->vip << "\n";
		MPI::COMM_WORLD.Send(&out, 1, MPI::CHAR, wakeFirstChild, 0);
	}
	wakeFirstChild++;
	if(wakeFirstChild < this->numThreads){
		///cout << "sending to child " << wakeFirstChild << " : " << this->vip << "\n";
		MPI::COMM_WORLD.Send(&out, 1, MPI::CHAR, wakeFirstChild, 0);
	}
	/*in this version of MCS Barrier, a sense variable is not necessary*/
	///cout << " " << this->vip <<" : finished barrier\n";
	return 0;
}

void MCSBarrier::resetChildNotReady()
{
	int j;
	for(j = 0; j < 4; j++)
	{
		this->childNotReady[j] = (((4*(this->vip))+j+1) < this->numThreads) ? 0x01 : 0x00;
	}
}

void MCSBarrier::print()
{
	int j;
	cout << " " << this->vip <<" : [ ";
	for(j = 0; j < 4; j++)
	{
		cout << (int)this->childNotReady[j];
		if(j + 1 == 4)
			cout << "]";
		else
			cout << ", ";
	}
	cout << "\n";
	cout << *((int*)this->childNotReady) << "\n";
}
