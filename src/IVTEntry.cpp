/*
 * IVTEntry.cpp
 *
 *  Created on: Sep 29, 2021
 *      Author: OS1
 */

#include "IVTEntry.h"

IVTEntry* IVTEntry::allEntries[256] = { 0 };
pInterrupt IVTEntry::interupts[256] = { 0 };

IVTEntry::IVTEntry(IVTNo ivtNo, pInterrupt pIntr) {
	this->ivtNo = ivtNo;
	this->oldIntr = 0;
	this->myEvent = 0;

	IVTEntry::allEntries[ivtNo] = this;
	IVTEntry::interupts[ivtNo] = pIntr;
}

IVTEntry::~IVTEntry() {
	this->ivtNo = 0;
	this->myEvent = 0;
	IVTEntry::allEntries[ivtNo] = 0;
	IVTEntry::interupts[ivtNo] = 0;
}

IVTEntry* IVTEntry::getEntry(IVTNo num) {
	return IVTEntry::allEntries[num];
}
