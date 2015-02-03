/*
 * Semaphore.cc
 *
 *  Created on: 4 Dec 2014
 *      Author: heisenberg
 */

#include <Semaphore.h>
#include <Timer.h>
#include <BOOOS.h>
#include <Task.h>

using namespace std;

namespace BOOOS{

Semaphore::Semaphore(int value){
	this->sem = value;
}
Semaphore::~Semaphore(){

	while(this->_waiting.length() > 0){
		Task * elem = (Task*)this->_waiting.remove();
		Task::self()->insert_ready(elem);
	}
	return;

}
void Semaphore::p(){
	Timer::control_on();
	this->sem--;

	if(this->sem < 0){
		this->_waiting.insert(Task::self());
		Timer::control_off();
		Task::self()->pass_to(Scheduler::self(), Task::WAITING);
	}
	Timer::control_on();

	return;

}
void Semaphore::v(){
	Timer::control_on();
	this->sem++;

	if(this->_waiting.length() > 0){
		Timer::control_off();
		Task  * elem_old = (Task*)this->_waiting.remove();
		Task::self()->insert_ready(elem_old);
	}
	Timer::control_on();
	return;
}

}


