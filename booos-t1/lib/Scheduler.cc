/*
 * BOOOS.h
 *
 * Scheduler.cc
 *
 *  Created on: 18 Sep 2014
 *      Author: guilherme
 */

#include "Scheduler.h"
#include <ucontext.h>
#include <Queue.h>
#include <iostream>
#include <BOOOS.h>

using namespace std;
namespace BOOOS {

Scheduler * Scheduler::__dispatcher = 0;

Scheduler::Scheduler() : Task::Task(dispatcher, 0, 0){

	this->_state = Task::SCHEDULER;
	Scheduler::__ready.remove();

}
Scheduler::~Scheduler(){}

void Scheduler::init(){
	if(Scheduler::__dispatcher == 0){
		Scheduler::__dispatcher = new Scheduler();
		return;
	}
}
void Scheduler::dispatcher(void*){

	Task * next;

	while(Task::__task_counter > 1){

		next = self()->choose_next();  // escolher a próxima Task* a executar

		if(next){
			if(BOOOS::SCHED_AGING){
				Scheduler::self()->aging();
			}
			// ações antes de lancar a tarefa "next", se houverem
			Scheduler::self()->pass_to(next); // transfere controle para a tarefa "next"
			// ações apos retornar da tarefa "next", se houverem
		}
	}

	Scheduler::self()->exit(0); // encerra a tarefa dispatcher
}
Task * Scheduler::choose_next(){

	return (Task*)__ready.remove();


}
//If the task is still on ready queue, task's priority "up".
void Scheduler::aging(){

	Queue::Element * aux;

	if(Task::__ready.length() == 0) return;

	int inside;
	for(aux = __ready.head()->next(), inside = 0; aux!=__ready.head()->next()|| inside == 0; aux = aux->next(), inside++){
		aux->rank(aux->rank()-1);
	}
	return;
}
void Scheduler::notify_time(Timer::Timestamp tricks){

	if(Task::self()->state() == Task::SCHEDULER){
		return;
	}else if(BOOOS::SCHED_POLICY == BOOOS::SCHED_ROUNDROBIN){
		Task::self()->yield();
	}


}

}
