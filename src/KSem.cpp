/*
 * KSem.cpp
 *
 *  Created on: Sep 29, 2021
 *      Author: OS1
 */

#include "KSem.h"
//#include "SemQ.h"
#include "SCHEDULE.H"

KernelSem::KernelSem(int init) {
	value = init;
	timerDeblockedThreads = 0;
	blockedOnNegVal = new ThreadQ();
	blockedOnSetTime = new ThreadQ();
}

KernelSem::~KernelSem() {
	lock;
	delete blockedOnNegVal;
	delete blockedOnSetTime;
	unlock;
}

int KernelSem::val() const {
	return value;
}

void KernelSem::inc(int a) {
	this->value+=a;
}

int KernelSem::wait(Time maxTimeToWait) {
	lock;
	value--;
	if (value < 0) {
		if (maxTimeToWait == 0) {
			PCB::running->status = BLOCKED;
			blockedOnNegVal->put((PCB*) PCB::running);
			unlock;
			TimerInt::dispatch();
			return 1;
		} else if (maxTimeToWait > 0) {
			PCB::running->status = BLOCKED;
			PCB::running->maxTime = maxTimeToWait;
			blockedOnSetTime->putSortedByMaxTime((PCB*) PCB::running);
			unlock;
			TimerInt::dispatch();
			return 2;
		}
	}
	unlock;
	return 0;
}

ThreadQ* KernelSem::getBlockedOnSetTime() {
	return blockedOnSetTime;
}

void KernelSem::signal() {
	lock;
	value++;
	if (value <= 0) {
		PCB* pcb = blockedOnSetTime->get();
		if (pcb == 0) {
			pcb = blockedOnNegVal->get();
			if (pcb == 0) {
				unlock;
				return;
			}
		}
		pcb->status = READY;
		Scheduler::put(pcb);
	}
	unlock;
}
