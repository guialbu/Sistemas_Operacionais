/*
 * Queue_Test.cc
 *
 *  Created on: Feb 21, 2014
 *      Author: arliones
 */

#include <iostream>
#include <sstream>
#include <queue>
#include <BOOOS.h>
#include <Scheduler.h>
#include "test_scaffold.h"

using namespace std;
using namespace BOOOS;

namespace Scheduler_Test_Functions {

	int test_init() {
		BOOOS::BOOOS::SCHED_POLICY = BOOOS::BOOOS::SCHED_PRIORITY;
		BOOOS::BOOOS::SCHED_PREEMPT = false;
		BOOOS::BOOOS::SCHED_AGING = false;
		BOOOS::BOOOS booos(false);

		ASSERT(Scheduler::self()->tid() == 1, 1);
		ASSERT(Scheduler::self()->state() == Task::SCHEDULER, 2);
		ASSERT(Task::task_counter() == 2, 3);

		Scheduler::init(); // should execute only once!
		ASSERT(Scheduler::self()->tid() == 1, 4);
		ASSERT(Scheduler::self()->state() == Task::SCHEDULER, 5);
		ASSERT(Task::task_counter() == 2, 6);

		Task::self()->exit(Task::self()->tid());

		return 0;
	}

	void f1(void* a) {}
	void f2(void* a) {}
	void f3(void* a) {}
	void f4(void* a) {}

	int test_creation_destruction() { // will add to Scheduler

		BOOOS::BOOOS::SCHED_POLICY = BOOOS::BOOOS::SCHED_ROUNDROBIN;
		BOOOS::BOOOS::SCHED_PREEMPT = false;
		BOOOS::BOOOS::SCHED_AGING = false;
		BOOOS::BOOOS booos(false);

		Task t1(f1, 1, (void*)"f1"), t2(f2, 1, (void*)"f2");
		Task * t3 = new Task(f3, 1, (void*)"f3");
		Task * t4 = new Task(f4, 1, (void*)"f4");

		ASSERT(t1.tid() == 2, 1);
		ASSERT(t1.state() == Task::READY, 2);
		ASSERT(t2.tid() == 3, 3);
		ASSERT(t2.state() == Task::READY, 4);
		ASSERT(t3->tid() == 4, 5);
		ASSERT(t3->state() == Task::READY, 6);
		ASSERT(t4->tid() == 5, 7);
		ASSERT(t4->state() == Task::READY, 8);
		ASSERT(Task::task_counter() == 6, 9);

		delete t3;
		delete t4;

		Task::self()->exit(Task::self()->tid());

		return 0;
	}


	queue<string> correct, log;

	void function_preempt(void * arg) {
		int i;

		{
			std::stringstream str;
			str << (char*)arg << " Start" << endl;
			log.push(str.str());
		}

		for(i=0; i<10; i++) {
			std::stringstream str;
			str << (char*)arg << " " << i << endl;
			log.push(str.str());
			Task::self()->nice(2*Task::self()->tid());
			Timer::delay(25000);
		}

		{
			std::stringstream str;
			str << (char*)arg << " End" << endl;
			log.push(str.str());
		}

		Task::self()->exit(Task::self()->tid());
	}

	int test_preempt_scheduling() {

		/* Expected test output */
		correct.push("Main Start\n");
		correct.push("Pang create\n");
		correct.push("\tPang Start\n");
		correct.push("\tPang 0\n");
		correct.push("Peng create\n");
		correct.push("\t\tPeng Start\n");
		correct.push("\t\tPeng 0\n");
		correct.push("Ping create\n");
		correct.push("\t\t\tPing Start\n");
		correct.push("\t\t\tPing 0\n");
		correct.push("Pong create\n");
		correct.push("\t\t\t\tPong Start\n");
		correct.push("\t\t\t\tPong 0\n");
		correct.push("Pung create\n");
		correct.push("\tPang 1\n");
		correct.push("\t\t\t\t\tPung Start\n");
		correct.push("\t\t\t\t\tPung 0\n");
		correct.push("Pang join\n");
		correct.push("\t\tPeng 1\n");
		correct.push("\t\t\tPing 1\n");
		correct.push("\t\t\t\tPong 1\n");
		correct.push("\tPang 2\n");
		correct.push("\t\t\t\t\tPung 1\n");
		correct.push("\t\tPeng 2\n");
		correct.push("\t\t\tPing 2\n");
		correct.push("\t\t\t\tPong 2\n");
		correct.push("\tPang 3\n");
		correct.push("\t\t\t\t\tPung 2\n");
		correct.push("\t\tPeng 3\n");
		correct.push("\t\t\tPing 3\n");
		correct.push("\t\t\t\tPong 3\n");
		correct.push("\tPang 4\n");
		correct.push("\t\t\t\t\tPung 3\n");
		correct.push("\t\tPeng 4\n");
		correct.push("\t\t\tPing 4\n");
		correct.push("\t\t\t\tPong 4\n");
		correct.push("\tPang 5\n");
		correct.push("\t\t\t\t\tPung 4\n");
		correct.push("\t\tPeng 5\n");
		correct.push("\t\t\tPing 5\n");
		correct.push("\t\t\t\tPong 5\n");
		correct.push("\tPang 6\n");
		correct.push("\t\t\t\t\tPung 5\n");
		correct.push("\t\tPeng 6\n");
		correct.push("\t\t\tPing 6\n");
		correct.push("\t\t\t\tPong 6\n");
		correct.push("\tPang 7\n");
		correct.push("\t\t\t\t\tPung 6\n");
		correct.push("\t\tPeng 7\n");
		correct.push("\t\t\tPing 7\n");
		correct.push("\t\t\t\tPong 7\n");
		correct.push("\tPang 8\n");
		correct.push("\t\t\t\t\tPung 7\n");
		correct.push("\t\tPeng 8\n");
		correct.push("\t\t\tPing 8\n");
		correct.push("\t\t\t\tPong 8\n");
		correct.push("\tPang 9\n");
		correct.push("\t\t\t\t\tPung 8\n");
		correct.push("\t\tPeng 9\n");
		correct.push("\t\t\tPing 9\n");
		correct.push("\t\t\t\tPong 9\n");
		correct.push("\tPang End\n");
		correct.push("\t\t\t\t\tPung 9\n");
		correct.push("Peng join\n");
		correct.push("\t\tPeng End\n");
		correct.push("\t\t\tPing End\n");
		correct.push("Ping join\n");
		correct.push("Pong join\n");
		correct.push("\t\t\t\tPong End\n");
		correct.push("Pung join\n");
		correct.push("\t\t\t\t\tPung End\n");
		correct.push("Main End\n");

		BOOOS::BOOOS::SCHED_POLICY = BOOOS::BOOOS::SCHED_ROUNDROBIN;
		BOOOS::BOOOS::SCHED_PREEMPT = true;
		BOOOS::BOOOS::SCHED_AGING = false;
		BOOOS::BOOOS booos(false);

		log.push("Main Start\n");

		Task::self()->nice(1);

		log.push("Pang create\n");
		Task * pang = new Task(function_preempt, 1, (char*)"\tPang");
		log.push("Peng create\n");
		Task * peng = new Task(function_preempt, 1, (char*)"\t\tPeng");
		log.push("Ping create\n");
		Task * ping = new Task(function_preempt, 1, (char*)"\t\t\tPing");
		log.push("Pong create\n");
		Task * pong = new Task(function_preempt, 1, (char*)"\t\t\t\tPong");
		log.push("Pung create\n");
		Task * pung = new Task(function_preempt, 1, (char*)"\t\t\t\t\tPung");


		log.push("Pang join\n");
		ASSERT(pang->join() == pang->tid(), 1);
		delete pang;
		log.push("Peng join\n");
		ASSERT(peng->join() == peng->tid(), 2);
		delete peng;
		log.push("Ping join\n");
		ASSERT(ping->join() == ping->tid(), 3);
		delete ping;
		log.push("Pong join\n");
		ASSERT(pong->join() == pong->tid(), 4);
		delete pong;
		log.push("Pung join\n");
		ASSERT(pung->join() == pung->tid(), 5);
		delete pung;

		log.push("Main End\n");

		ASSERT(log.size() == correct.size(), 6);

		string str1,str2;
		int i = 7;
		while(log.size()) {
			str1 = log.front(); log.pop();
			str2 = correct.front(); correct.pop();
			ASSERT(str1 == str2, i);
			i++;
		}

		return 0;
	}

	void function_preempt_aging(void * arg) {
		int i;

		{
			std::stringstream str;
			str << (char*)arg << " Start" << endl;
			log.push(str.str());
		}

		for(i=0; i<10; i++) {
			std::stringstream str;
			str << (char*)arg << " " << i << endl;
			log.push(str.str());
			Timer::delay(25000);
		}

		{
			std::stringstream str;
			str << (char*)arg << " End" << endl;
			log.push(str.str());
		}

		Task::self()->exit(Task::self()->tid());
	}

	int test_preempt_aging_scheduling() {

		/* Expected test output */
		correct.push("Main Start\n");
		correct.push("Pang create\n");
		correct.push("\tPang Start\n");
		correct.push("\tPang 0\n");
		correct.push("Peng create\n");
		correct.push("Ping create\n");
		correct.push("Pong create\n");
		correct.push("Pung create\n");
		correct.push("Pang join\n");
		correct.push("\t\tPeng Start\n");
		correct.push("\t\tPeng 0\n");
		correct.push("\t\t\tPing Start\n");
		correct.push("\t\t\tPing 0\n");
		correct.push("\t\t\t\tPong Start\n");
		correct.push("\t\t\t\tPong 0\n");
		correct.push("\t\t\t\t\tPung Start\n");
		correct.push("\t\t\t\t\tPung 0\n");
		correct.push("\tPang 1\n");
		correct.push("\t\tPeng 1\n");
		correct.push("\t\t\tPing 1\n");
		correct.push("\t\t\t\tPong 1\n");
		correct.push("\t\t\t\t\tPung 1\n");
		correct.push("\tPang 2\n");
		correct.push("\t\tPeng 2\n");
		correct.push("\t\t\tPing 2\n");
		correct.push("\t\t\t\tPong 2\n");
		correct.push("\t\t\t\t\tPung 2\n");
		correct.push("\tPang 3\n");
		correct.push("\t\tPeng 3\n");
		correct.push("\t\t\tPing 3\n");
		correct.push("\t\t\t\tPong 3\n");
		correct.push("\t\t\t\t\tPung 3\n");
		correct.push("\tPang 4\n");
		correct.push("\t\tPeng 4\n");
		correct.push("\t\t\tPing 4\n");
		correct.push("\t\t\t\tPong 4\n");
		correct.push("\t\t\t\t\tPung 4\n");
		correct.push("\tPang 5\n");
		correct.push("\t\tPeng 5\n");
		correct.push("\t\t\tPing 5\n");
		correct.push("\t\t\t\tPong 5\n");
		correct.push("\t\t\t\t\tPung 5\n");
		correct.push("\tPang 6\n");
		correct.push("\t\tPeng 6\n");
		correct.push("\t\t\tPing 6\n");
		correct.push("\t\t\t\tPong 6\n");
		correct.push("\t\t\t\t\tPung 6\n");
		correct.push("\tPang 7\n");
		correct.push("\t\tPeng 7\n");
		correct.push("\t\t\tPing 7\n");
		correct.push("\t\t\t\tPong 7\n");
		correct.push("\t\t\t\t\tPung 7\n");
		correct.push("\tPang 8\n");
		correct.push("\t\tPeng 8\n");
		correct.push("\t\t\tPing 8\n");
		correct.push("\t\t\t\tPong 8\n");
		correct.push("\t\t\t\t\tPung 8\n");
		correct.push("\tPang 9\n");
		correct.push("\t\tPeng 9\n");
		correct.push("\t\t\tPing 9\n");
		correct.push("\t\t\t\tPong 9\n");
		correct.push("\t\t\t\t\tPung 9\n");
		correct.push("\tPang End\n");
		correct.push("Peng join\n");
		correct.push("\t\tPeng End\n");
		correct.push("\t\t\tPing End\n");
		correct.push("\t\t\t\tPong End\n");
		correct.push("\t\t\t\t\tPung End\n");
		correct.push("Ping join\n");
		correct.push("Pong join\n");
		correct.push("Pung join\n");
		correct.push("Main End\n");

		BOOOS::BOOOS::SCHED_POLICY = BOOOS::BOOOS::SCHED_ROUNDROBIN;
		BOOOS::BOOOS::SCHED_PREEMPT = true;
		BOOOS::BOOOS::SCHED_AGING = true;
		BOOOS::BOOOS booos(false);

		log.push("Main Start\n");

		Task::self()->nice(1);


		log.push("Pang create\n");
		Task * pang = new Task(function_preempt_aging, 1, (char*)"\tPang");
		log.push("Peng create\n");
		Task * peng = new Task(function_preempt_aging, 1, (char*)"\t\tPeng");
		log.push("Ping create\n");
		Task * ping = new Task(function_preempt_aging, 1, (char*)"\t\t\tPing");
		log.push("Pong create\n");
		Task * pong = new Task(function_preempt_aging, 1, (char*)"\t\t\t\tPong");
		log.push("Pung create\n");
		Task * pung = new Task(function_preempt_aging, 1, (char*)"\t\t\t\t\tPung");

		log.push("Pang join\n");
		ASSERT(pang->join() == pang->tid(), 1);
		delete pang;
		log.push("Peng join\n");
		ASSERT(peng->join() == peng->tid(), 2);
		delete peng;
		log.push("Ping join\n");
		ASSERT(ping->join() == ping->tid(), 3);
		delete ping;
		log.push("Pong join\n");
		ASSERT(pong->join() == pong->tid(), 4);
		delete pong;
		log.push("Pung join\n");
		ASSERT(pung->join() == pung->tid(), 5);
		delete pung;

		log.push("Main End\n");

		ASSERT(log.size() == correct.size(), 6);

		string str1,str2;
		int i = 7;
		while(log.size()) {
			str1 = log.front(); log.pop();
			str2 = correct.front(); correct.pop();
			ASSERT(str1 == str2, i);
			i++;
		}

		return 0;
	}
}

int main() {
	cout << "Welcome to BOOOS - Basic Object Oriented Operating System!" << endl;
	cout << "This program will test the class: Scheduler configured with Priority Policy using Join" << endl;

	// For debugging, call the function directly and comment the unit testing block above.
//	cout << Scheduler_Test_Functions::test_preempt_scheduling() << endl;
//	return 0;

	UnitTest tests("Prio_Scheduler_Join");

	tests.attach_test(&Scheduler_Test_Functions::test_init, "Init");
	tests.attach_test(&Scheduler_Test_Functions::test_creation_destruction, "Creation and Destruction");
	tests.attach_test(&Scheduler_Test_Functions::test_preempt_scheduling, "Priority with Preemption, without Aging");
	tests.attach_test(&Scheduler_Test_Functions::test_preempt_aging_scheduling, "Priority with Preemption and Aging");

	tests.run();

	return 0;
}
