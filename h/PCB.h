#ifndef PCB_H_
#define PCB_H_

#include "Thread.h"
#include "ThreadQ.h"
#include "IdleThr.h"
#include <dos.h>

#define lock;      \
 		asm pushf; \
 		asm cli;

#define unlock; asm popf;

enum Status{NEW, RUNNING, FINISHED, READY, BLOCKED};

class PCB {
public:
	unsigned* st;
	volatile StackSize stackSize;
	volatile unsigned sp;
	volatile unsigned ss;
	volatile unsigned bp;
	volatile int quantum;
	unsigned int maxTime; //za semafore
	volatile int infiniteFlag;
	volatile Status status;
	static ID poslID;
	ID id;

	friend class Thread;
	Thread* myThread;
	volatile static ThreadQ* allThreads;
	volatile static PCB* running;
	static IdleThr* idleThread; // volatile ???
	volatile ThreadQ* onWait;

	void waitToComplete();
	void start();
	static void body();
	void createStack(StackSize stack);
	PCB(Thread * thread, StackSize stack, Time t);
	virtual ~PCB();
};

#endif /* PCB_H_ */
