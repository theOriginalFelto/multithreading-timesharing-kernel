/*
 * TimerInt.h
 *
 *  Created on: Sep 27, 2021
 *      Author: OS1
 */

#ifndef TIMERINT_H_
#define TIMERINT_H_

class TimerInt {
public:
	static volatile int context_switch_on_demand;

	static void inic();
	static void restore();
	static void dispatch();

	virtual ~TimerInt();
	TimerInt();
};

#endif /* TIMERINT_H_ */
