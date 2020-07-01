#include "System.h"
#include "Schedule.h"
#include <stdio.h>
#include "ListPCB.h"
#include "PCB.h"

int ListPCB::getSize() const{
	return size;
}

void ListPCB::add(PCB* p){
	lock
	Elem* tmp = new Elem(p);
	last = (first == 0 ? first : last->next) = tmp;
	size++;
	unlock
}

PCB* ListPCB::removeFirst(){
	Elem* tmp = first;
	PCB* tmpPCB = 0;
	lock
	if(tmp == 0){
		unlock
		return 0;
	}
	tmpPCB = tmp->info;
	first = first->next;

	tmp->next = 0;
	delete tmp;
	size--;

	if(first == 0)
		last = 0;

	unlock
	return tmpPCB;

}
void ListPCB::removeByID(ID id){
	Elem* temp = first;
	Elem* prev = 0;
	lock
	while(temp){
		if(temp->info->pid == id){
			Elem* old = temp;
			if(prev == 0)
				first = temp->next;
			else
				prev->next = temp->next;
			if(last == old)
				last = prev;
			size--;
			temp = temp->next;
			old->next = 0;
			delete old;
		}
		else{
			prev = temp;
			temp = temp->next;
		}
	}
	unlock

}

Thread* ListPCB::findThreadByID(ID id) {
	Elem* temp = first;
	lock
	if(first == 0){
		unlock
		return 0;
	}
	while(temp->info->pid != id){
		temp = temp->next;
	}
	unlock
	return temp->info->myThread;
}

ListPCB::~ListPCB() {
	Elem* pom = 0;
	lock
	while(first){
		pom = first;
		first = first->next;
		pom->next = 0;
		delete pom;
	}
	size = 0;
	first = 0;
	last = 0;
	unlock
}


ListPCB& ListPCB::restoreBlocked(){
	Elem* tmpE = 0;
	PCB* tmpP = 0;
	lock
	while(size > 0){
		tmpE = first;
		tmpP = tmpE->info;

		first = first->next;

		tmpE->info->status = PCB::READY;
		Scheduler::put(tmpP);
		tmpE->next = 0;
		delete tmpE;
		size--;
	}
	first = last = 0;
	unlock
	return *this;
}


