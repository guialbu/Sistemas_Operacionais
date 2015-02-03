/*
 * Scheduler.h
 *
 *  Created on: Mar 21, 2014
 *      Author: guilherme
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <Task.h>
#include <Queue.h>
#include <Timer.h>

namespace BOOOS {

class Scheduler : public Task {
	friend class Task;

protected:
	Scheduler();

public:
	enum SchedulerType {
		SCHED_FCFS,
		SCHED_PRIORITY,
		SCHED_ROUNDROBIN
	};

	virtual ~Scheduler();

	static void init();

	static void dispatcher(void*);

	static Scheduler * self() { return __dispatcher; }

	static void aging();

	static void notify_time(Timer::Timestamp tricks);

protected:
	virtual Task * choose_next();

	static Scheduler * __dispatcher;
};

} /* namespace BOOOS */

#endif /* SCHEDULER_H_ */
