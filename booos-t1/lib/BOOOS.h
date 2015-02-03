/*
 * BOOOS.h
 *
 *  Created on: Aug 14, 2014
 *      Author: arliones
 */

#ifndef BOOOS_H_
#define BOOOS_H_

#include <Scheduler.h>

namespace BOOOS {

/* Configuration Space */

class BOOOS {
public:
	BOOOS(bool verbose = true);
	~BOOOS();

	static const int BOOOS_VERSION = 0;

	enum SCHED_TYPE {
		SCHED_FCFS,
		SCHED_PRIORITY,
		SCHED_ROUNDROBIN
	};
	static SCHED_TYPE SCHED_POLICY;
	static bool SCHED_PREEMPT;
	static bool SCHED_AGING;
	static Timer::Timestamp TIMER_RESOLUTION;
	static Timer::Timestamp TASK_QUANTUM;
	static Timer::Timestamp SCHEDULER_RESOLUTION;
	static BOOOS::SCHED_TYPE SCHEDULER_TYPE;
	static const int  MESSAGE_SIZE = 100;

	bool is_verbose() { return _verbose; }
	static void panic();
	static BOOOS * self() { return __booos; }

private:
	bool _verbose;
	static BOOOS * __booos;
};

} /* namespace BOOOS */

#endif /* BOOOS_H_ */
