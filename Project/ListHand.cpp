/*
 * ListSig.cpp
 *
 *  Created on: Apr 14, 2020
 *      Author: OS1
 */

#include "ListHand.h"


ListHand::~ListHand() {
//	printf("Delete List: %d\n", size);
	HandlerElement* tmp;
	while(first){
		tmp = first;
		first = first -> next;
		size--;
		delete tmp;
	}
	first = last = 0;
	size = 0;
}

void ListHand::add(SignalHandler handlerFunction){
	HandlerElement* tmp = new HandlerElement(handlerFunction);
	last = (first == 0 ? first : last->next) = tmp;
	size++;
	/*if(first == 0)
		printf("Bad add");
	else{
		if(last == 0){
			printf("Another bad add");
		}
		else
			printf("SignalHandler add, at position: %d\n", size);
	}*/
}

int ListHand::getSize(){
	return size;
}
void ListHand::copy(ListHand* lh){
	HandlerElement *tmp = lh->first;
//	printf("copy: %d in %d->", lh->getSize(), size);
	while(tmp != 0){
		this->add(tmp->handler);
		tmp = tmp->next;
	}
//	printf("%d\n", size);
}

void ListHand::handleAllInList() {
// useful printfs
//	printf("Num handlers: %d\n", size);
	HandlerElement* tmp = first;
	while(tmp){
		(*tmp->handler)();
		tmp = tmp->next;
	}
}


void ListHand::unregisterAll(){
	HandlerElement* tmp;
	while(first){
		tmp = first;
		first = first -> next;
		size--;
		delete tmp;
	}
	first = last = 0;

//	printf("%d \n", size);
	size = 0;
}

void ListHand::swapFunctions(SignalHandler hand1, SignalHandler hand2){
	HandlerElement* tmp = first;
	HandlerElement* h1 = 0, *h2 = 0;
	while(tmp){
		if(tmp->handler == hand1){
			h1 = tmp;
		}
		else if(tmp->handler == hand2){
				h2 = tmp;
			}
		tmp = tmp->next;
	}
	if(h1 != 0 && h2 != 0){
		h1->handler = hand2;
		h2->handler = hand1;
	}
}
