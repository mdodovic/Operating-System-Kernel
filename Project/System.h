#ifndef SYSTEM_H_
#define SYSTEM_H_
#include "Thread.h"
typedef void interrupt (*InterruptRoutine)(...);

#define lock asm pushf; asm cli; {}
#define unlock asm popf; {}

class Thread;
class PCB;

class System {
public:

	static Thread* mainThread;
	static Idle* idle;

	static void interrupt timer(...);
	static InterruptRoutine old;
	static volatile int contextSwitchOnDemand;
	static volatile int lockFlag;

	static void inic();
	static void restore();
};

#endif /* SYSTEM_H_ */
