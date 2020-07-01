#include "IVTEntry.h"


#define PREPAREENTRY(numEntry, callOldIntrInd)\
\
void interrupt intr##numEntry(...);\
\
IVTEntry newEntry##numEntry(numEntry, intr##numEntry);\
\
void interrupt intr##numEntry(...){\
\
	(IVTEntry::IVTable[##numEntry])->signalEvent();\
\
	if (callOldIntrInd)\
		newEntry##numEntry.myOldRoutine();\
	dispatch();\
}\
