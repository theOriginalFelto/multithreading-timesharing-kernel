/*
 * IVTEntry.h
 *
 *  Created on: Sep 29, 2021
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "KernelEv.h"

typedef unsigned char IVTNo;
typedef void interrupt (*pInterrupt)(...);

#define PREPAREENTRY(num, flag)\
	void interrupt intr##num(...);\
	IVTEntry newEntry##num((unsigned char)num, (pInterrupt)intr##num);\
	void interrupt intr##num(...) {\
		newEntry##num.myEvent->signal();\
		if (flag == 1) newEntry##num.oldIntr();\
	}

class KernelEv;

class IVTEntry {
public:
	KernelEv* myEvent;
	pInterrupt oldIntr;
	static IVTEntry* allEntries[256];
	static pInterrupt interupts[256];

	static IVTEntry* getEntry(IVTNo num);

	IVTEntry(IVTNo ivtNo, pInterrupt pIntr);
	~IVTEntry();
protected:
	friend class KernelEv;
private:
	IVTNo ivtNo;
};

#endif /* IVTENTRY_H_ */
