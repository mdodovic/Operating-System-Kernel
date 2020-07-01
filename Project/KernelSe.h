#ifndef KERNELSE_H_
#define KERNELSE_H_
typedef unsigned int Time;

class ListPCB;
class KernelSem {
	static volatile KernelSem* first;
	static volatile KernelSem* last;
	KernelSem* next;

	int value;
	ListPCB* blockedList;
	ListPCB* timeBlockedList;

public:
	friend class PCB;
	friend class System;
	friend class SemList;

	KernelSem(int init);
	virtual ~KernelSem();

	int wait(Time maxTimeToWait);
	int signal(int n);
	int val() const;

	static void updateSleepLists();
};

#endif /* KERNELSE_H_ */
