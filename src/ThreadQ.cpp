/*
 * ThreadQ.cpp
 *
 *  Created on: Sep 27, 2021
 *      Author: OS1
 */

#include "ThreadQ.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include "KSem.h"

ThreadQ::~ThreadQ() {
	while(front){
		temp = front;
		front = front->next;
		lock;
		delete temp;
		unlock;
	}
}

ThreadQ::ThreadQ() {
	front = rear = 0;
}

void ThreadQ::put(PCB* p) {
	if (rear == 0) {
		lock;
	    rear = new Elem();
	    unlock;
	    rear->next = 0;
	    rear->pcb = p;
	    front = rear;
	} else {
		lock;
	    temp = new Elem();
	    unlock;
	    rear->next = temp;
	    temp->pcb = p;
	    temp->next = 0;
	    rear = temp;
	}
}

void ThreadQ::putSorted(PCB* p) {
	if (rear == 0) { // ako je lista prazna
		lock;
	    rear = new Elem();
	    unlock;
	    rear->next = 0;
	    rear->pcb = p;
	    front = rear;
	} else {
		Elem* tren = front;
		Elem* prev = 0;
		while(tren != 0 && tren->pcb->id < p->id) { // trazimo mjesto za element
			prev = tren;
			tren = tren->next;
		}
		lock;
	    temp = new Elem(); // novi element
	    unlock;
		temp->pcb = p;
		if (tren == 0) { // na kraj liste dodajemo
			rear->next = temp;
			temp->next = 0;
	    	rear = temp;
		} else {
			temp->next = tren;
			if(prev != 0) prev->next = temp;
			else front = temp;
		}
	}
}

void ThreadQ::putSortedByMaxTime(PCB* p) {
	if (rear == 0) { // ako je lista prazna
		lock;
	    rear = new Elem();
	    unlock;
	    rear->next = 0;
	    rear->pcb = p;
	    front = rear;
	} else {
		Elem* tren = front;
		Elem* prev = 0;
		while(tren != 0 && tren->pcb->maxTime < p->maxTime) { // trazimo mjesto za element
			prev = tren;
			tren = tren->next;
		}
		lock;
	    temp = new Elem(); // novi element
	    unlock;
		temp->pcb = p;
		if (tren == 0) { // na kraj liste dodajemo
			rear->next = temp;
			temp->next = 0;
	    	rear = temp;
		} else {
			temp->next = tren;
			if(prev != 0) prev->next = temp;
			else front = temp;
		}
	}
}

PCB* ThreadQ::get() {
	lock;
	temp = front;
	PCB* p;
	if (temp != 0) {
		if(temp->next != 0) {
			p = temp->pcb;
			temp = temp->next;
			delete front;
			front = temp;
		} else {
			p = front->pcb;
			delete front;
			front = 0;
			rear = 0;
		}
		unlock;
		return p;
	} else {
		unlock;
		return 0;
	}
}

Thread* ThreadQ::search(ID id) {
	temp = front;
	ID ident = 0;
	while(temp) {
		ident = temp->pcb->myThread->getId();
		if(ident == id) {
			return temp->pcb->myThread;
		}
		else temp = temp->next;
	}
	return 0;
}

int ThreadQ::checkIfUnblocked(KernelSem* ks) {
	int a = 0;
	Elem* tempo = front;
	PCB* p;
	while (tempo) {
		p = tempo->pcb;
		tempo = tempo->next;
		p->maxTime--;
		if (p->maxTime == 0) {
			//p->status = READY;
			//Scheduler::put((PCB*)p);
			ks->signal();
			a++;
		}
	}
	return a;
}
