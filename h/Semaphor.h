#ifndef SEMAPHOR_H_
#define SEMAPHOR_H_

#include "PCB.h"
#include "SemQ.h"

typedef unsigned int Time;

class KernelSem;
class Semaphore {
public:
	Semaphore (int init=1);
	virtual ~Semaphore ();

	virtual int wait (Time maxTimeToWait);
	virtual void signal();

	int val () const; // Returns the current value of the semaphore
	volatile static SemQ* allSemaphores;
private:
	KernelSem* myImpl;
};
#endif /* SEMAPHOR_H_ */
