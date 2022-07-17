/*
 * KernelEv.cpp
 *
 *  Created on: Sep 29, 2021
 *      Author: OS1
 */

#include "KernelEv.h"
#include "PCB.h"
#include "TimerInt.h"
#include "SCHEDULE.H"

KernelEv::KernelEv(IVTNo ivtNo) {
	this->owner = (PCB*) PCB::running;
	this->blocked = 0;

	this->myEntry = IVTEntry::allEntries[ivtNo];
	myEntry->myEvent = this;

#ifndef BCC_BLOCK_IGNORE
	myEntry->oldIntr = getvect(ivtNo);
	setvect(ivtNo, IVTEntry::interupts[ivtNo]);
#endif
}

KernelEv::~KernelEv() {
#ifndef BCC_BLOCK_IGNORE
	setvect(myEntry->ivtNo, myEntry->oldIntr);
#endif
	this->owner = this->blocked = 0;
	delete myEntry;
}

void KernelEv::wait() {
	lock;
	if (this->owner != PCB::running) {
		unlock;
	} else {
		owner->status = BLOCKED;
		blocked = owner;
		unlock;
		TimerInt::dispatch();
	}
}

void KernelEv::signal() {
	lock;
	if (blocked == 0) {
		unlock;
	} else {
		blocked->status = READY;
		Scheduler::put(blocked);
		blocked = 0;
		unlock;
		TimerInt::dispatch();
	}
}
