#ifndef PCB_H_
#define PCB_H_

#include "Thread.h"
#include "ListSign.h"
#include "ListHand.h"
class ListPCB;

class PCB {
public:
	unsigned int ss;
	unsigned int sp;
	unsigned int bp;

	enum Status {READY, CREATED, FINISHED, BLOCKED};
	volatile Status status;

	Time timeToUse;
	volatile Time timeLeft;
	Time timeSemaphore;
	int infinityWaitFlag;

	int size;
	unsigned *stack;

	static int stId;
	int pid;

	Thread *myThread;
	ID parentThreadId;

	ListPCB* listOfPCBwaitForMe;
	static ListPCB* listOfAllPCB;
	static volatile PCB* running;
	static void wrapper();

	PCB(Thread* thread, StackSize stackSize, Time timeSlice_);

	virtual ~PCB();

	void createStack();


	ListHand* listOfHandlers [16];
	ListSign* listOfArrivedSignals;
	static int* globallyBlockedSignals;
	int blockedSignals[16];


	void signal(SignalId signal);

	void registerHandler(SignalId signal, SignalHandler handler);
	void unregisterAllHandlers(SignalId id);

	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);

	void blockSignal(SignalId signal);
	static void blockSignalGlobally(SignalId signal);

	void unblockSignal(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);

	void checkArrivedSignals();


};

#endif /* PCB_H_ */
