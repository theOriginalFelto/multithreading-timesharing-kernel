#include "Thread.h"
#include "PCB.h"
#include "ThreadQ.h"
#include "TimerInt.h"

Thread::Thread (StackSize stackSize, Time timeSlice) {
	lock;
	myPCB = new PCB(this, stackSize, timeSlice);
	unlock;
	((ThreadQ*) PCB::allThreads)->put((PCB*) myPCB);
}

Thread::~Thread (){
	waitToComplete();
	delete myPCB;
}

PCB* Thread::getPCB() {
	return myPCB;
}

ID Thread::getId() {
	return myPCB->id;
}

ID Thread::getRunningId() {
	return PCB::running->id;
}

Thread* Thread::getThreadById(ID id) {
	Thread* th = ((ThreadQ*)PCB::allThreads)->search(id);
	return th;
}

void Thread::waitToComplete() {
	if (this == PCB::idleThread) {
		PCB::idleThread->resetInf();
	}
	if (this != PCB::running->myThread) {
		myPCB->waitToComplete();
	}
}

void Thread::start() {
	myPCB->start();
}

void dispatch() {
	TimerInt::dispatch();
}
