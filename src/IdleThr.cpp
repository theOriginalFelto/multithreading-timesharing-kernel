/*
 * IdleThr.cpp
 *
 *  Created on: Sep 28, 2021
 *      Author: OS1
 */

#include "IdleThr.h"

IdleThr::IdleThr() : Thread(defaultStackSize, 1) {
	this->inf = 1;
}

void IdleThr::run() {
	while(this->inf);
}

void IdleThr::resetInf() {
	this->inf = 0;
}
