#ifndef IVTENTRY_H_
#define IVTENTRY_H_
#include "System.h"
#include "Event.h"

class IVTEntry {

	friend class KernelEv;
	KernelEv* myEvent;
	IVTNo myEntryNum;

//	IVTEntry::IVTable[##numEntry]->callOldRoutine();
// In case myOldRoutine in prepare entry and destructor doesn't work

public:
	IVTEntry(IVTNo ivtNo, InterruptRoutine novaRutina);
	virtual ~IVTEntry();

	//void callOldRoutine();
	// In case myOldRoutine doesn't work
	void signalEvent();

	static IVTEntry* IVTable[256];
	InterruptRoutine myOldRoutine;
};

#endif /* IVTENTRY_H_ */
