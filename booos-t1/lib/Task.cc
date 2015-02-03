/*
 * BOOOS.h
 *
 *  Created on: Aug 14, 2014
 *      Author: guilherme
 */
#include "Task.h"
#include "BOOOS.h"
#include <ucontext.h>
#include <Scheduler.h>
#include <Timer.h>
#include <iostream>

using namespace std;

namespace BOOOS {

volatile Task * Task:: __running;
volatile Task * Task:: __main;
Queue Task::__ready;
int Task:: __tid_counter;
int const Task:: STACK_SIZE;
int  Task::__task_counter;


// Task constructor: must initialize task's attributes
// - entry_point: pointer to a function that implements the behavior of the task
// - nargs: number of arguments sent to the entry_point function
// - arg: vector with the arguments for the entry_point function
Task::Task(void (*entry_point)(void *), int nargs, void * arg) {

	getcontext(&this->_context);
	this->_state = READY;

	this->_stack = new char[STACK_SIZE];
	/* allocates memory for stack*/

	if(this->_stack) {
		this->_context.uc_stack.ss_sp = this->_stack;
		this->_context.uc_stack.ss_size = STACK_SIZE;
		this->_context.uc_stack.ss_flags = 0;
		this->_context.uc_link = 0;
	} else {
		cerr << "Error:: Stack can't be created " << endl;
		return;
	}

	makecontext(&this->_context, (void (*)()) entry_point, nargs, arg);
	this->_tid = __tid_counter;
	Task::__tid_counter++;
	Task::__task_counter++;
	this->priority = 0;
	this->rank(0);
	this->_time_exec = 0;
	this->_time_init = Timer::time();
	this->_time_old = 0;
	this->_time_res = 0;
	this->_ativ = 0;

	this->insert_ready(this);

	if(BOOOS::SCHED_PREEMPT){
		if(this->rank() < Task::self()->rank()){
			Task::self()->yield();
			return;
		}
	}
	return;

}
// Task constructor: must initialize main's attributes
Task::Task() {

	//warning because the stack wasn't declared
	getcontext(&_context);
	_tid = __tid_counter;
	priority = 0;
	rank(0);
	_time_exec = 0;
	_time_init = Timer::time();
	_time_old = 0;
	_ativ = 1;
	_time_res = 0;
	return;

}
// Task destructor: must finalize task's attributes
Task::~Task(){

	if (this->_state != FINISHING){
		Task::exit(0);
	}

	delete [] this->_stack;
	return;

}
// salve the context of current object (this)
// and load on the task is being passed by parameter
// * t: pointer to task will assume CPU
// s: state that current task will assume
void Task::pass_to(Task * t, State s) {

	this->nice(this->priority);

	this->_time_exec += Timer::time() - this->_time_old;

	if(this->_state != Task::SCHEDULER){
		this->_state = s;
	}
	Task::__running = t;
	if(t->state() != Task::SCHEDULER){
		t->_state = RUNNING;
	}
	if(this->_state == READY){
		this->insert_ready(this);
	}

	t->_ativ++;
	t->_time_old = Timer::time();

	this->_time_res = Timer::time() - this->_time_init;

	if(s == FINISHING){
		cout << "Task " << this->tid()
						<< " exit: response time " << (this->_time_res)/1000
						<< " ms, CPU time " << (this->_time_exec)/1000 << " ms, "
						<< this->_ativ << " activations" << endl;
	}

	swapcontext(&this->_context, &t->_context);
	return;

}
// forge the first task (main)
void Task::init(){

	Task::__tid_counter = 0;
	Task::__task_counter = 0;
	Task::__main = new Task();
	__main->_state = RUNNING;
	Task::__tid_counter++;
	Task::__task_counter++;
	Task::__running = Task::__main;
	return;

}
//exit
void Task::exit(int code){

	Task * elem;

	while(this->_join.length()!=0){
		elem = (Task*)this->_join.remove();
		elem->_state = READY;
		this->insert_ready(elem);
	}
	this->_code = code;
	__task_counter--;

	if(this->state() == SCHEDULER){
		this->pass_to((Task*)__main, FINISHING);
	}else{
		this->pass_to((Task*)Scheduler::self(), FINISHING);
	}

	return;

}
//transfers execution of the current task to the scheduler
void Task::yield(){

	this->pass_to(Scheduler::self(), READY);
	return;

}
//set rank for task - priority
void Task::nice(int p){

	if(-20 <= p <= 20){
		this->rank(p);
		this->priority = this->rank();
	}
	return;
}
int Task::join(){

	if(this == 0) return -1;

	if(this->_state == FINISHING){
		return this->_code;
	}
	else{
		this->_join.insert(Task::self());
		Task::self()->pass_to(Scheduler::self(), JOIN);
		return this->_code;
	}
}
// Insert the taks on ready queue - FCFS & Priority & Round-robin
void Task::insert_ready(Task * t){

	if(BOOOS::SCHED_POLICY == BOOOS::SCHED_PRIORITY){
		Task::__ready.insert_ordered(t); // Priority
	} else {
		Task::__ready.insert(t); // FCFS && Round-Robin
	}
	return;
}
}




