/*
 * ThreadQ.h
 *
 *  Created on: Sep 27, 2021
 *      Author: OS1
 */

#ifndef THREADQ_H_
#define THREADQ_H_

#include "Thread.h"
#include <iostream.h>

struct Elem {
	PCB* pcb;
	Elem *next;
};

class KernelSem;

class ThreadQ {
private:
	Elem* front;
	Elem* rear;
	Elem* temp;
public:
	void put(PCB*);
	void putSorted(PCB*);
	void putSortedByMaxTime(PCB*);
	PCB* get();
	Thread* search(ID id); //opsluzava metodu u Thread
	int checkIfUnblocked(KernelSem* ks); //za semafore

	virtual ~ThreadQ();
	ThreadQ();
};

#endif /* THREADQ_H_ */
