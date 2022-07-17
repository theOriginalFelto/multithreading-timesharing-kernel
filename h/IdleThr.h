/*
 * IdleThr.h
 *
 *  Created on: Sep 28, 2021
 *      Author: OS1
 */

#ifndef IDLETHR_H_
#define IDLETHR_H_

#include "Thread.h"

class IdleThr: public Thread {
public:
	void run();
	IdleThr();
	void resetInf();
private:
	volatile int inf;
};

#endif /* IDLETHR_H_ */
