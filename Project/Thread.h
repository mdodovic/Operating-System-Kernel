#ifndef THREAD_
#define THREAD_
typedef unsigned long StackSize;
const StackSize defaultStackSize = 500; //4096;
typedef unsigned int Time;
const Time defaultTimeSlice = 2;
typedef int ID;

typedef void (*SignalHandler)();
typedef unsigned SignalId;


class PCB;

class Thread{

public:

	void start();

	void waitToComplete();

	virtual ~Thread();

	ID getId();
	static ID getRunningId();
	static Thread* getThreadById(ID id);

	void signal(SignalId signal);

	void registerHandler(SignalId signal, SignalHandler handler);
	void unregisterAllHandlers(SignalId id);

	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);

	void blockSignal(SignalId signal);
	static void blockSignalGlobally(SignalId signal);

	void unblockSignal(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);


protected:
	friend class PCB;
	friend class Idle;
	friend class System;
	friend class KernelSem;
	Thread(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual void run() {}

private:
	PCB* myPCB;

};

void dispatch();

#endif /* THREAD_ */
