/*
 * KSem.h
 *
 *  Created on: Sep 29, 2021
 *      Author: OS1
 */

#ifndef KSEM_H_
#define KSEM_H_

#include "ThreadQ.h"
#include "TimerInt.h"
#include "PCB.h"


class KernelSem {
public:
	int wait (Time maxTimeToWait);
	void signal();

	int val () const; // Returns the current value of the semaphore
	void inc(int a);
	ThreadQ* getBlockedOnSetTime();
	KernelSem(int init=1);
	~KernelSem();

	int timerDeblockedThreads;
private:
	int value;
	ThreadQ* blockedOnNegVal;
	ThreadQ* blockedOnSetTime;
};

#endif /* KSEM_H_ */
