/*
 * Semaphor.cpp
 *
 *  Created on: Sep 29, 2021
 *      Author: OS1
 */

#include "Semaphor.h"

volatile SemQ* Semaphore::allSemaphores = new SemQ();

Semaphore::Semaphore(int init) {
	lock;
	myImpl = new KernelSem(init);
	unlock;
	((SemQ*)Semaphore::allSemaphores)->put(myImpl);
}

Semaphore::~Semaphore() {
	lock;
	delete myImpl;
	unlock;
}

int Semaphore::wait(Time t) {
	return myImpl->wait(t);
}

void Semaphore::signal() {
	myImpl->signal();
}

int Semaphore::val() const {
	return myImpl->val();
}
