/*
 * TimerInt.cpp
 *
 *  Created on: Sep 27, 2021
 *      Author: OS1
 */

#include "TimerInt.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include "IdleThr.h"
#include "Semaphor.h"

volatile unsigned oldTimerOFF, oldTimerSEG;
volatile unsigned tsp;
volatile unsigned tss;
volatile unsigned tbp;
volatile unsigned int cntr = 2;
volatile int flag = 0; //ako se nit izvršava "neograniceno"

volatile int TimerInt::context_switch_on_demand = 0;

TimerInt::TimerInt() {
	inic();
}

TimerInt::~TimerInt() {}

void tick();

void checkIfUnblockedOnSems() {
	((SemQ*) Semaphore::allSemaphores)->checkIfUnblocked();
}

void interrupt timer() {
	if (!TimerInt::context_switch_on_demand) {
		cntr--;
		checkIfUnblockedOnSems(); // oslobadjanje niti koje su zablokirane na dato vrijeme na semaforima
	}

	if (cntr == 0 || TimerInt::context_switch_on_demand) {
		if (!flag || TimerInt::context_switch_on_demand) {
#ifndef BCC_BLOCK_IGNORE
			asm {
				// cuva sp, ss i bp
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}
#endif

			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = tbp;

			// stavi nit u scheduler, osim ako je Idle
			if (PCB::running != ((IdleThr*) PCB::idleThread)->getPCB()) {
				if ((PCB::running->status) == RUNNING) {
					PCB::running->status = READY;
					Scheduler::put((PCB*) PCB::running);
				}
			}

			// uzmi nit iz scheduler-a
			PCB::running = Scheduler::get();
			if (!PCB::running) {
				PCB::running = ((IdleThr*) PCB::idleThread)->getPCB();
			} else {
				PCB::running->status = RUNNING;
			}

			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;

			cntr = PCB::running->quantum;
			flag = PCB::running->infiniteFlag;
#ifndef BCC_BLOCK_IGNORE
			asm {
				// restaurira sp
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}
#endif
		}
	}

	// poziv stare prekidne rutine
	// koja se nalazila na 08h, a sad je na 60h;
	// poziva se samo kada nije zahtevana promena konteksta
	// tako da se stara rutina poziva
	// samo kada je stvarno doslo do prekida

	if (!TimerInt::context_switch_on_demand) {
#ifndef BCC_BLOCK_IGNORE
		asm int 60h;
		tick();
#endif
	}

	TimerInt::context_switch_on_demand = 0;
}

void TimerInt::inic() {
#ifndef BCC_BLOCK_IGNORE
	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax // es = 0

		// pamti staru rutinu
		mov ax, word ptr es:0022h
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		// postavlja novu rutinu
		mov word ptr es:0022h, seg timer
		mov word ptr es:0020h, offset timer

		// postavlja staru rutinu na int 60h
		mov ax, oldTimerSEG
		mov word ptr es:0182h, ax
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}
#endif
}

void TimerInt::restore() {
#ifndef BCC_BLOCK_IGNORE
	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax

		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
#endif
}

void TimerInt::dispatch() {
	lock;
	TimerInt::context_switch_on_demand = 1;
	timer();
	unlock;
}

