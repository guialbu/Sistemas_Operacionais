/*
 * Task.h
 *
 *  Created on: Aug 15, 2014
 *      Author: guilherme
 */

#ifndef TASK_H_
#define TASK_H_

#include <Queue.h>
#include <ucontext.h>


namespace BOOOS {

class Task : public Queue::Element {
public:
	enum State {
		READY,
		WAITING,
		RUNNING,
		FINISHING,
		SCHEDULER,
		JOIN
	};

	Task(void (*entry_point)(void *), int nargs, void * arg);
	virtual ~Task();

	int tid() { return _tid; }
	static int task_counter() { return __task_counter; }
	State state() { return _state; }
	void pass_to(Task * t, State s = READY);
	void exit(int code);
	void yield();
	void nice(int p);
	void insert_ready(Task * t);
	int join();

	static Task * self() { return (Task *)__running; }
	static void init();


private:

	Task();

	static volatile Task * __running;
	static volatile Task * __main;
	static int __tid_counter;
	static int const STACK_SIZE = 32768;
	int _tid; // task ID
	ucontext_t _context;
	char * _stack;

protected:

	static Queue __ready;
	static int __task_counter;
	State _state;
	Queue _join;
	int _code;
	int priority;
	int _time_init;
	int _time_exec;
	int _time_old;
	int _time_res;
	int _ativ;

};

} /* namespace BOOOS */

#endif /* TASK_H_ */
