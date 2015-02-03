/*
 * Timer.h
 *
 *  Created on: Mar 30, 2014
 *      Author: arliones
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <Queue.h>

namespace BOOOS {

class Scheduler;

class Timer {

protected:
	Timer() {}

public:

	typedef unsigned long long Timestamp;

	virtual ~Timer() {}

	static void init(Scheduler * sched, Timestamp period);
	static void start();

	static Timestamp ticks();
	static Timestamp time();

	static void delay_ticks(Timestamp ticks);
	static void delay(Timestamp microseconds);


	static void control_on(){__control = true;}
	static void control_off(){__control = false;}

private:
	static void sig_handler(int signum);

	static Timestamp __ticks;

	static itimerval __timer;
	static struct sigaction __action;

	static Scheduler * __scheduler;
	static Timestamp __period;
	static bool __control;
};

} /* namespace BOOOS */

#endif /* TIMER_H_ */
