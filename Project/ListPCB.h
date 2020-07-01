#ifndef LISTPCB_H_
#define LISTPCB_H_
#include "Thread.h"

class ListPCB {

	struct Elem{
		PCB* info;
		Elem* next;
		Elem (PCB* pcb_):info(pcb_),next(0){}
	};

	friend class PCB;
	friend class System;
	friend class ListSEM;
	friend class KernelSem;
	Elem *first, *last;
	int size;

public:
	ListPCB():first(0), last(0), size(0) {}
	virtual ~ListPCB();

	int getSize() const;

	void add(PCB* p);

	PCB* removeFirst();
	void removeByID(ID id);

	Thread* findThreadByID (ID id);

	ListPCB& restoreBlocked();

};

#endif /* LISTPCB_H_ */
