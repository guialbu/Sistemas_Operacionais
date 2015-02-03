/*
 * Timer.cc
 *
 *  Created on: 12 Dec 2014
 *      Author: heisenberg
 */


#include "Timer.h"
#include <BOOOS.h>
#include <Scheduler.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

namespace BOOOS{

Timer::Timestamp Timer::__ticks = 0;
itimerval Timer::__timer;
struct sigaction Timer::__action;
Scheduler * Timer::__scheduler;
Timer::Timestamp Timer::__period;
bool Timer::__control;


void Timer::init(Scheduler * sched, Timestamp perio){

	__scheduler = sched;
	__period = perio;
	__ticks = 0;
	Timer::control_off();

	__timer.it_value.tv_usec = BOOOS::TIMER_RESOLUTION;
	__timer.it_value.tv_sec = 0;
	__timer.it_interval.tv_usec = BOOOS::TIMER_RESOLUTION;
	__timer.it_interval.tv_sec = 0;

	__action.sa_handler = sig_handler;
	sigemptyset(&__action.sa_mask);
	__action.sa_flags = 0;

	if(sigaction(SIGALRM, &__action, 0) < 0){
		cerr << "Erro em signatcion: " << endl;
		exit(1);
	}

	return;

}
void Timer::delay(Timestamp microsseconds){

	Timer::Timestamp timer = Timer::time();
	while((Timer::time() - timer) <= microsseconds);
	return;

}
void Timer::delay_ticks(Timestamp ticks){

	Timer::Timestamp current_tick  = Timer::ticks();
	while((Timer::ticks() - current_tick) <= ticks);
	return;

}
void Timer::start(){

	if (setitimer (ITIMER_REAL, &__timer, 0) < 0)
	{
		cerr << "Erro em setitimer: " << endl;
		exit (1) ;
	}
	return;
}
Timer::Timestamp Timer::ticks(){

	return Timer::__ticks;

}
Timer::Timestamp Timer::time(){

	return (ticks() * BOOOS::TIMER_RESOLUTION);

}
void Timer::sig_handler(int signum){

	Timer::__ticks++;

	if(__control == false){
		if((Timer::__ticks % Timer::__period) == 0){
			Timer::__scheduler->notify_time(Timer::ticks());
		}
	}
	return;
}

}





