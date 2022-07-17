//pretpostavljeni memorijski model: huge
#include <iostream.h>
#include <dos.h>
#include "SCHEDULE.H"
#include "PCB.h"
#include "TimerInt.h"

// mozda problem sa idle (waitToComplete) -> rijeseno TESTIRAJ
// mozda problem sa preranim lock; u PCB::body -> nista ovo
// EVENTUALNO uvecavanje interne sem prom. u checkIfUnblocked

int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]){
	PCB* main = new PCB(0, 4096, 2);
	main->status = RUNNING;
	PCB::running = main;

	TimerInt* timer = new TimerInt();

	int ret = userMain(argc, argv);

	TimerInt::restore();
	delete timer;

	return ret;
}
