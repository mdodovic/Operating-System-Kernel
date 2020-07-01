#ifndef LISTSIGN_H_
#define LISTSIGN_H_

#include "Thread.h"
#include "STDIO.H"
class ListSign {
	struct SignalElement{

		SignalId signalId;
		SignalElement* next;

		SignalElement(SignalId id): signalId(id), next(0) {}
	};
	SignalElement *first, *last;
	int size;
public:
	ListSign():first(0), last(0), size(0){}

	virtual ~ListSign();
	int getSize();
	void copy(ListSign* ls);

	void add(SignalId id);

	void handleAllSignals();

};

#endif /* LISTSIGN_H_ */
