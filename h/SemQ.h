/*
 * SemQ.h
 *
 *  Created on: Sep 29, 2021
 *      Author: OS1
 */

#ifndef SEMQ_H_
#define SEMQ_H_

#include "KSem.h"

struct Elem_ {
	KernelSem * ksem;
	Elem_ *next;
};

class SemQ {
private:
	Elem_* front;
	Elem_* rear;
	Elem_* temp;
public:
	void put(KernelSem * ks);
	KernelSem * get();

	void checkIfUnblocked();
	~SemQ();
	SemQ();
};

#endif /* SEMQ_H_ */
