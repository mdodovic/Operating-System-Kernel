#include "KernelEv.h"
#include "Schedule.h"
#include "System.h"
#include "stdio.h"

KernelEv::KernelEv(IVTNo ivtNo): value(0) {
	ownerOfEvent = (PCB*) PCB::running;
	IVTEntry::IVTable[ivtNo]->myEvent = this;
	entryForMe = IVTEntry::IVTable[ivtNo];
}

KernelEv::~KernelEv() {
	entryForMe->myEvent = 0;
	ownerOfEvent = 0;
}

void KernelEv::wait(){

	if (ownerOfEvent != PCB::running){
		return;
	}
	if (value == 1){
		value--;
		return;
	}
	if (value == 0) {
		value--;
		ownerOfEvent->status = PCB::BLOCKED;
		dispatch();
	}
	return;
}

void KernelEv::signal(){
	if (value == 1){
		return; // Binary semaphore cannot be greater than 1
	}
	if (value == 0){
		value++; // 2 signals without midblocking!
		return;
	}
	value = 0; // value was -1, and thread was blocked
	ownerOfEvent->status = PCB::READY;
	Scheduler::put(ownerOfEvent);
}



