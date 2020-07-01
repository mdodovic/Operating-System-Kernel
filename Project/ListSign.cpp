/*
 * ListSign.cpp
 *
 *  Created on: Apr 17, 2020
 *      Author: OS1
 */
#include "ListSign.h"
#include "STDIO.H"
#include "PCB.h"
#include "ListHand.h"
#include "System.h"
#include "Thread.h"

ListSign::~ListSign(){
	SignalElement* tmp;
	while(first){
		tmp = first;
		first = first -> next;
		size--;
		delete tmp;
	}
	first = last = 0;
	size = 0;
}

int ListSign::getSize(){
	return size;
}

void ListSign::copy(ListSign* ls){
	SignalElement *tmp = ls->first;
	while(tmp != 0){
		this->add(tmp->signalId);
		tmp = tmp->next;
	}
}

void ListSign::add(SignalId id){
	SignalElement* tmp = new SignalElement(id);
	last = (first == 0 ? first : last->next) = tmp;
	size++;

}

extern void handler0();

void ListSign::handleAllSignals(){
	SignalElement* temp = first, *prev = 0;
	SignalElement* old;
	while(temp){
		int skipBlocked = 0;
		if(temp->signalId == 0){
			if(PCB::globallyBlockedSignals[0] == 0){
				if(PCB::running->blockedSignals[0] == 0){
						handler0();
				}
				else{
					skipBlocked = 1;
				}
			}
			else{
				skipBlocked = 1;
			}
		}
		else{
			if(PCB::globallyBlockedSignals[temp->signalId] == 0){
				if(PCB::running->blockedSignals[temp->signalId] == 0){
					PCB::running->listOfHandlers[temp->signalId]->handleAllInList();
				}
				else{
					skipBlocked = 1;
				}
			}
			else{
				skipBlocked = 1;
			}
		}
		if(skipBlocked){
			prev = temp;
			temp = temp->next;
		}
		else{
			old = temp;
			temp = temp->next;
			if(old == first)
				first = temp;
			else
				prev->next = temp;

			if(last == old)
				last = prev;

			old->next = 0;
			size--;
			delete old;
		}
	}

	if(PCB::running->status == PCB::FINISHED){
		System::lockFlag = 1;
		dispatch();
	}
}

