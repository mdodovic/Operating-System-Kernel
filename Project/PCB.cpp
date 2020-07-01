#include "PCB.h"
#include <dos.h>

#include "ListPCB.h"
#include "System.h"

ID PCB::stId=0;
ListPCB* PCB::listOfAllPCB = new ListPCB();
volatile PCB* PCB::running=0;



PCB::PCB(Thread* thread, StackSize stackSize, Time timeSlice_) :
		timeToUse(timeSlice_), timeLeft(timeSlice_), timeSemaphore(0),
		infinityWaitFlag(0), ss(0), bp(0), sp(0), stack(0), status(CREATED){

	myThread = thread;
	listOfPCBwaitForMe = new ListPCB();

	pid=stId++;

	parentThreadId = -1;

	if(running != 0){
//		printf("Thread %d is parent to thread %d\n", running->pid, pid);
		parentThreadId = running->pid;
	}
	/*else{
		printf("I (%d) am my parent\n", pid);
	}*/

	size = stackSize/sizeof(unsigned);
	if(stackSize >= 65536)
		size = 65536/sizeof(unsigned);
	createStack();

	listOfArrivedSignals = new ListSign();
	for(int i = 0; i < 16; i++){
		listOfHandlers[i] = new ListHand();
		blockedSignals[i] = 0;
	}
	if(running != 0){
		listOfArrivedSignals->copy(Thread::getThreadById(parentThreadId)->myPCB->listOfArrivedSignals);
		for(int i = 0; i < 16; i++){
			listOfHandlers[i]->copy(Thread::getThreadById(parentThreadId)->myPCB->listOfHandlers[i]);
			blockedSignals[i] = Thread::getThreadById(parentThreadId)->myPCB->blockedSignals[i];
		}
	}
/*	{//all threads
		printf("Thread %d initial condition", pid);
		for(int i = 0; i < 16; i++){
			printf("List %d, child ->%d\n", i, listOfHandlers[i]->getSize());
		}
		for(int j = 0; j < 16; j++){
			printf("%d ", blockedSignals[j]);
		}
		printf("\n");
	}*/
}


void PCB::createStack(){
	lock
	stack = new unsigned[size];

#ifndef BCC_BLOCK_IGNORE
	stack[size - 1] = 0x200; //PCW-I = 1
	stack[size - 2] = FP_SEG(&(wrapper)); // cs
	stack[size - 3] = FP_OFF(&(wrapper)); // ip
	// Stack pointer
	this->ss = FP_SEG(stack + size - 12);
	this->sp = FP_OFF(stack + size - 12);
	this->bp = FP_OFF(stack + size - 12);
#endif

  unlock
}


void PCB::wrapper(){

	//printf("Wrapper: %d\n", pid);

	running->myThread->run();
	//Here thread is FINISHED!
	lock
	//printf("Thread %d finished!\n", pid);
	if(running->parentThreadId != -1){
		Thread::getThreadById(running->parentThreadId)->signal(1);
	}
	if(PCB::globallyBlockedSignals[2] == 0){
		if(PCB::running->blockedSignals[2] == 0){
			PCB::running->listOfHandlers[2]->handleAllInList();
		}
	}
	running->status = PCB::FINISHED;
	running->listOfPCBwaitForMe->restoreBlocked();

	unlock
	dispatch();

}

void handler0(){
//	printf("Before kill\n");
	if(PCB::running -> parentThreadId != -1){
		Thread::getThreadById(PCB::running -> parentThreadId)->signal(1);
	}

	if(PCB::globallyBlockedSignals[2] == 0){
		if(PCB::running->blockedSignals[2] == 0){
			PCB::running->listOfHandlers[2]->handleAllInList();
		}
	}

	PCB::running->status = PCB::FINISHED;
	PCB::running->listOfPCBwaitForMe->restoreBlocked();

//	printf("After kill\n");
}

PCB::~PCB() {

	lock
//	printf("PCB %d deleting", pid);
	delete listOfPCBwaitForMe;
	delete [] stack;

	delete listOfArrivedSignals;
	for(int i = 0; i < 16; i++){
		delete listOfHandlers[i];
	}

	unlock
}

void PCB::signal(SignalId signal){
	if(this->pid == running->pid){
//		printf("I(%d) am in my context: signal(%d)\n",running->pid, signal);
		if(globallyBlockedSignals[signal] == 0){
			if(blockedSignals[signal] == 0){
				listOfHandlers[signal]->handleAllInList();
			}
		}

	}
	else{
//		printf("I(%d!=%d) NOT IN CONTEXT: signal(%d)\n",running->pid, pid, signal);
		listOfArrivedSignals->add(signal);
	}
}

void PCB::checkArrivedSignals(){
	// comming from interrupt after context change
	listOfArrivedSignals->handleAllSignals();

}



void PCB::registerHandler(SignalId signal, SignalHandler handler){
	listOfHandlers[signal]->add(handler);
}

void PCB::blockSignal(SignalId signal){
	this->blockedSignals[signal] = 1;
}

void PCB::blockSignalGlobally(SignalId signal){
	globallyBlockedSignals[signal] = 1;
}

void PCB::unblockSignal(SignalId signal){
	this->blockedSignals[signal] = 0;
}

void PCB::unblockSignalGlobally(SignalId signal){
	globallyBlockedSignals[signal] = 0;
}

void PCB::unregisterAllHandlers(SignalId id){
	this->listOfHandlers[id]->unregisterAll();
}

void PCB::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){
	this->listOfHandlers[id]->swapFunctions(hand1, hand2);
}
