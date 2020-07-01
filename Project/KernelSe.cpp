#include "System.h"
#include "PCB.h"
#include "SCHEDULE.h"
#include <stdio.h>
#include "Thread.h"
#include "Idle.h"
#include "KernelSe.h"
#include "ListPCB.h"
volatile KernelSem* KernelSem::first = 0;
volatile KernelSem* KernelSem::last = 0;


KernelSem::KernelSem(int init) {
	lock
	value = init;
	next = 0;
	blockedList = new ListPCB();
	timeBlockedList = new ListPCB();

	last = (first == 0 ? first : last->next) = this;
	unlock

}

KernelSem::~KernelSem() {
	lock
	delete blockedList;
	delete timeBlockedList;
	KernelSem* tmp = (KernelSem*) first;
	KernelSem* preth = 0;
	while(tmp){
		if(tmp == this){
			if(preth == 0){
				first = tmp->next;
			}
			else
				preth->next = tmp->next;

			if(last == tmp)
				last = preth;
			tmp = tmp->next;
			break;
		}
		else{
			preth = tmp;
			tmp = tmp->next;
		}
	}
	unlock
}


int KernelSem::wait(Time maxTimeToWait){
	int ret = 0;
	lock
	if (--value < 0){
		if (maxTimeToWait == 0){
			PCB::running->infinityWaitFlag = 1;
			ret = 1;
			PCB::running->status = PCB::BLOCKED;

			blockedList->add((PCB *)PCB::running);
			unlock
			dispatch();
		}
		else{
			PCB::running->infinityWaitFlag = 0;
			ret = 0;
			PCB::running->status = PCB::BLOCKED;
			PCB::running->timeSemaphore = maxTimeToWait;

			timeBlockedList->add((PCB *)PCB::running);
			blockedList->add((PCB *)PCB::running);
			unlock
			dispatch();
			if(PCB::running->infinityWaitFlag == 1)
				ret = 1;
		}
		return ret;
	}
	unlock
	return 1;

}

int KernelSem::signal(int n){
	lock
	if (n==0){
		if(value++<0){
			PCB* unblockedThread = blockedList->removeFirst();
			if(unblockedThread->infinityWaitFlag == 0)
				timeBlockedList->removeByID(unblockedThread->pid);
			unblockedThread->status = PCB::READY;
			unblockedThread->infinityWaitFlag = 1;
			Scheduler::put(unblockedThread);
			unlock
			return 0;
		}
	}
	else if( n >= 1 ){
		int cnt = 0;
		value += n;
		if(blockedList->getSize() > n){
			cnt = n;
		}
		else{
			cnt = blockedList->getSize();
		}
		for(int i = 0; i < cnt; i++){
			PCB* unblockedThread = blockedList->removeFirst();
			if(unblockedThread->infinityWaitFlag == 0)
				timeBlockedList->removeByID(unblockedThread->pid);
			unblockedThread->status = PCB::READY;
			unblockedThread->infinityWaitFlag = 1;
			Scheduler::put(unblockedThread);
		}
		unlock
		return cnt;
	}
	unlock
	return n;
}

int KernelSem::val() const {
	return value;
}


void KernelSem::updateSleepLists(){
	ListPCB::Elem* nextInList;
	lock
	for(KernelSem* semaphors = (KernelSem *)first;semaphors ; semaphors = semaphors->next) {
		for(ListPCB::Elem* temp = semaphors->timeBlockedList->first; temp; temp = nextInList) {
			nextInList = temp->next;
			PCB* pom = temp->info;
			pom->timeSemaphore--;
			if(pom->timeSemaphore == 0) {
				semaphors->blockedList->removeByID(pom->pid);     // temp is already changed
				semaphors->timeBlockedList->removeByID(pom->pid); // so it is safe to remove
				semaphors->value++;

				pom->infinityWaitFlag = 0;
				pom->status = PCB::READY;
				Scheduler::put(pom);
			}
		}
	}
	unlock
}
