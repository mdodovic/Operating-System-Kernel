#include "IVTEntry.h"
#include "KernelEv.h"
#include <stdio.h>
#include <dos.h>
IVTEntry* IVTEntry::IVTable[256];

IVTEntry::IVTEntry(IVTNo ivtNo, InterruptRoutine intRoutine):
		myOldRoutine(0), myEntryNum(ivtNo), myEvent(0) {
	lock
	IVTable[ivtNo] = this;
#ifndef BCC_BLOCK_IGNORE
	this->myOldRoutine = getvect(ivtNo);
	setvect(ivtNo, intRoutine);
#endif
	unlock
}


IVTEntry::~IVTEntry() {
	lock
	(*myOldRoutine) (); // Be careful
#ifndef BCC_BLOCK_IGNORE
	setvect(myEntryNum, myOldRoutine);
#endif
	IVTable[myEntryNum] = 0;
	unlock
}

/*In case line 21: (*myOldRoutine) () doesn't work
 * void IVTEntry::callOldRoutine(){
 * lock
 * (*myOldRoutine) ();
 * unlock
 * }
*/
void IVTEntry::signalEvent(){
	if (myEvent) {
		myEvent->signal();
	}
}
