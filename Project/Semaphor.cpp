#include "Semaphor.h"
#include "KernelSe.h"
#include "System.h"

Semaphore::Semaphore(int init) {
	lock
	myImpl = new KernelSem(init);
	unlock
}
Semaphore::~Semaphore() {
	lock
	delete myImpl;
	unlock
}

int Semaphore::wait(Time maxTimeToWait){
	int retWait = 0;
	lock
	retWait = myImpl->wait(maxTimeToWait);
	unlock
	return retWait;
}


int Semaphore::signal(int n){
	int retSignal = 0;
	lock
	retSignal = myImpl->signal(n);
	unlock
	return retSignal;
}

int Semaphore::val() const{
	int retVal = 0;
	lock
	retVal = myImpl->val();
	unlock
	return retVal;
}

