#ifndef KERNELEV_H_
#define KERNELEV_H_
#include "PCB.h"
#include "System.h"
#include "Event.h"
#include "IVTEntry.h"

class KernelEv {
	int value;
	PCB* ownerOfEvent;
	IVTEntry* entryForMe;

public:
	KernelEv(IVTNo ivtNo);
	virtual ~KernelEv();

	void wait();
	void signal();
};

#endif /* KERNELEV_H_ */
