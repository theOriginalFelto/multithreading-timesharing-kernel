/*
 * KernelEv.h
 *
 *  Created on: Sep 29, 2021
 *      Author: OS1
 */

#ifndef KERNELEV_H_
#define KERNELEV_H_

#include "IVTEntry.h"
#include "Event.h"


class IVTEntry;
class PCB;

class KernelEv {
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();

	void wait ();
	void signal();

	PCB* owner;
	PCB* blocked;
	IVTEntry* myEntry;
};

#endif /* KERNELEV_H_ */
