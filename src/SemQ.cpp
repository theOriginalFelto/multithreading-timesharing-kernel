/*
 * SemQ.cpp
 *
 *  Created on: Sep 29, 2021
 *      Author: OS1
 */

#include "SemQ.h"


SemQ::~SemQ() {
	while(front){
		temp = front;
		front = front->next;
		lock;
		delete temp;
		unlock;
	}
}

SemQ::SemQ() {
	front = rear = 0;
}

void SemQ::put(KernelSem* ks) {
	if (rear == 0) {
		lock;
	    rear = new Elem_();
	    unlock;
	    rear->next = 0;
	    rear->ksem = ks;
	    front = rear;
	} else {
		lock;
	    temp = new Elem_();
	    unlock;
	    rear->next = temp;
	    temp->ksem = ks;
	    temp->next = 0;
	    rear = temp;
	}
}

KernelSem* SemQ::get() {
	temp = front;
	KernelSem* ks;
	if (temp != 0) {
		if(temp->next != 0) {
			ks = temp->ksem;
			temp = temp->next;
			lock;
			delete front;
			unlock;
			front = temp;
		} else {
			ks = front->ksem;
			lock;
			delete front;
			unlock;
			front = 0;
			rear = 0;
		}
		return ks;
	} else return 0;
}

void SemQ::checkIfUnblocked() {
	temp = front;
	KernelSem* ks;
	int deblocked;
	while (temp) {
		ks = temp->ksem;
		ThreadQ* b = ks->getBlockedOnSetTime();
		deblocked = b->checkIfUnblocked(ks);
		//ks->timerDeblockedThreads+=deblocked;
		//ks->inc(deblocked);
		temp = temp->next;
	}
}
