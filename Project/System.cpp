#include "System.h"
#include "PCB.h"
#include "SCHEDULE.h"
#include "Idle.h"
#include <dos.h>
#include <stdio.h>

#include "KernelSe.h"
#include "ListPCB.h"

volatile int System::contextSwitchOnDemand=0;
volatile int System::lockFlag=1;
InterruptRoutine System::old = 0;
int* PCB::globallyBlockedSignals = new int[16];

Idle* System::idle = 0;
Thread* System::mainThread = 0;

extern void tick();


void System::inic(){

	lock

#ifndef BCC_BLOCK_IGNORE
	old = getvect(0x8);
	setvect(0x8,&timer);
	setvect(0x60, old);
#endif

	for(int i = 0; i < 16; i++){
		PCB::globallyBlockedSignals[i] = 0;
	}

	mainThread = new Thread();
	PCB::running = (volatile PCB*) mainThread->myPCB;
	PCB::running->status = PCB::READY;

	idle = new Idle();
	idle->myPCB->status = PCB::READY;
	unlock
}

void System::restore(){
	lock

#ifndef BCC_BLOCK_IGNORE
	setvect(0x8, old);
#endif

	delete idle;
	delete mainThread;
	lockFlag = 0;
	unlock
}


void interrupt System::timer(...){

	static unsigned tsp=0, tss=0, tbp=0;

	if (contextSwitchOnDemand==0)	{
		KernelSem::updateSleepLists();
		tick();
		asm int 60h;
	}

	if(contextSwitchOnDemand == 0 && PCB::running->timeToUse != 0){
		PCB::running->timeLeft--;
	}

	if (contextSwitchOnDemand==1 || (PCB::running->timeLeft==0 && PCB::running->timeToUse !=0)){
		if (lockFlag == 1){
			contextSwitchOnDemand = 0;
			asm {
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}

			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = tbp;

			//printf("Run: %d", PCB::running->pid);
			if (PCB::running->status == PCB::READY && PCB::running != System::idle->myPCB) {
				Scheduler::put((PCB*) PCB::running);
			}

			PCB::running = Scheduler::get();

			if (PCB::running == 0){
				PCB::running= System::idle->myPCB;
			}
			//printf("->: %d\n", PCB::running->pid);

			PCB::running->timeLeft = PCB::running->timeToUse;

			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;
			asm{
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}
			if(PCB::running->pid != System::idle->myPCB->pid)
				PCB::running->myThread->myPCB->checkArrivedSignals();
		}
		else{
			contextSwitchOnDemand = 1;
		}

	}
}


