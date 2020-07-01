#ifndef LISTHAND_H_
#define LISTHAND_H_

#include "Thread.h"
#include "STDIO.H"
class ListHand {

	struct HandlerElement{
		SignalHandler handler;
		HandlerElement* next;
		HandlerElement(SignalHandler handlerFunction): handler(handlerFunction), next(0) {}
	};
	HandlerElement *first, *last;
	int size;

public:
	ListHand():first(0), last(0), size(0) {}
	virtual ~ListHand();
	void add(SignalHandler handlerFunction);
	int getSize();
	void copy(ListHand* lh);
	void handleAllInList();
	void unregisterAll();
	void swapFunctions(SignalHandler hand1, SignalHandler hand2);

};

#endif /* LISTHAND_H_ */
