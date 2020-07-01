#include "Thread.h"
#include "PCB.h"
#include "System.h"
#include "SCHEDULE.h"
#include "Idle.h"
#include "ListPCB.h"


Thread::Thread(StackSize stackSize, Time timeSlice){
	lock
	myPCB = new PCB(this, stackSize, timeSlice);
	PCB::listOfAllPCB->add(this->myPCB);
	unlock
}

ID Thread::getRunningId(){
	return PCB::running->pid;
}

ID Thread::getId(){
	return myPCB->pid;
}

Thread* Thread::getThreadById(ID id) {
	Thread* threadToBeFound = 0;
	lock
	threadToBeFound = PCB::listOfAllPCB->findThreadByID(id);
	unlock
	return threadToBeFound;
}

void Thread::start(){
	lock
	if(myPCB->status == PCB::CREATED){
		myPCB->status = PCB::READY;
		Scheduler::put(myPCB);
	}
	unlock
}

void Thread::waitToComplete(){
	lock
	if ( (this->myPCB->status!=PCB::FINISHED) && (this->myPCB->status != PCB::CREATED)){
		// thread is operational
		if(PCB::running != (volatile PCB*)this->myPCB){
			// thread isn't call itself
			if((this != System::idle)){
				// thread isn't idle
				PCB::running->status = PCB::BLOCKED;
				myPCB->listOfPCBwaitForMe->add((PCB*)PCB::running);
				unlock
				dispatch();
				return;
			}
		}
	}
	unlock
}


Thread::~Thread(){
	lock
	PCB::listOfAllPCB->removeByID(myPCB->pid);
	myPCB->listOfPCBwaitForMe->restoreBlocked(); //
	delete myPCB;
	unlock
}


void dispatch(){
	lock
	System::contextSwitchOnDemand = 1;
	System::timer();
	unlock
}


void Thread::signal(SignalId signal){
	if(signal >= 16)
		return;
	System::lockFlag = 0;
	this->myPCB->signal(signal);
	System::lockFlag = 1;
}

void Thread::registerHandler(SignalId signal, SignalHandler handler){
	if(signal >= 16)
		return;
	System::lockFlag = 0;
	this->myPCB->registerHandler(signal,handler);
	System::lockFlag = 1;
}
void Thread::unregisterAllHandlers(SignalId id){
	if(id >= 16)
		return;
	System::lockFlag = 0;
	this->myPCB->unregisterAllHandlers(id);
	System::lockFlag = 1;
}

void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){
	if(id >= 16)
		return;
	System::lockFlag = 0;
	this->myPCB->swap(id, hand1, hand2);
	System::lockFlag = 1;
}

void Thread::blockSignal(SignalId signal){
	if(signal >= 16)
		return;
	System::lockFlag = 0;
	this->myPCB->blockSignal(signal);
	System::lockFlag = 1;
}
void Thread::blockSignalGlobally(SignalId signal){
	if(signal >= 16)
		return;
	System::lockFlag = 0;
	PCB::blockSignalGlobally(signal);
	System::lockFlag = 1;
}

void Thread::unblockSignal(SignalId signal){
	if(signal >= 16)
		return;
	System::lockFlag = 0;
	this->myPCB->unblockSignal(signal);
	System::lockFlag = 1;

}
void Thread::unblockSignalGlobally(SignalId signal){
	if(signal >= 16)
		return;
	System::lockFlag = 0;
	PCB::unblockSignalGlobally(signal);
	System::lockFlag = 1;
}


