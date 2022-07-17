/*
 * PCB.cpp
 *
 *  Created on: Sep 21, 2021
 *      Author: OS1
 */

#include "PCB.h"
#include "ThreadQ.h"
#include "SCHEDULE.H"
#include "TimerInt.h"

ID PCB::poslID = 0;
volatile PCB* PCB::running = 0;
IdleThr* PCB::idleThread = new IdleThr();
volatile ThreadQ* PCB::allThreads = new ThreadQ();

PCB::~PCB() {
	delete[] st;
	delete onWait;
}

PCB::PCB(Thread * thread, StackSize stack, Time t) {
	myThread = thread;
	createStack(stack);
	quantum = t;
	maxTime = 0;
	if (t == 0) {
		infiniteFlag = 1;
	} else {
		infiniteFlag = 0;
	}
	id = ++poslID;
	status = NEW;
	lock;
	onWait = new ThreadQ();
	unlock;
}

void PCB::createStack(StackSize stack) {
	stackSize = stack;
	if (stackSize > 65535) {
		stackSize = 65535;
	}
	stackSize /= sizeof(unsigned);
	st = new unsigned[stackSize];
	st[stackSize - 1] = 0x200;
#ifndef BCC_BLOCK_IGNORE
	st[stackSize - 2] = FP_SEG(body);
	st[stackSize - 3] = FP_OFF(body);

	sp = FP_OFF(st+stackSize-12);
	ss = FP_SEG(st+stackSize-12);
	bp = sp;
#endif
}

void PCB::start() {
	lock;
	if (status == NEW) {
		status = READY;
		Scheduler::put(this);
	}
	unlock;
}

void PCB::body() {
	running->myThread->run();
	lock;
	running->status = FINISHED;
	while (1) {
		PCB* curr = ((ThreadQ*) running->onWait)->get();
		if (!curr) {
			break;
		}
		curr->status = READY;
		Scheduler::put(curr);
	}
	/*if (PCB::running == ((IdleThr*) PCB::idleThread)->getPCB()) {
		delete PCB::idleThread;
		PCB::idleThread = new IdleThr();
	}*/
	unlock;
	TimerInt::dispatch();
}

void PCB::waitToComplete() {
	lock;
	if (status != FINISHED && status != NEW) {
		running->status = BLOCKED;
		((ThreadQ*) onWait)->put((PCB*) running);
		unlock;
		TimerInt::dispatch();
	} else {
		unlock;
	}
}
