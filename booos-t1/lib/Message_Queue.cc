#include <Message_Queue.h>
#include <BOOOS.h>
#include <Queue.h>
#include <Semaphore.h>

using namespace std;
namespace BOOOS {

Message_Queue::Message_Queue(int max_size) {

	this->_max_size = max_size;
	this->empty = new Semaphore(0);
	this->full = new Semaphore(this->_max_size);

}
Message_Queue::~Message_Queue() {

	while (this->_queue.length()) {
		delete this->_queue.remove();
	}
}

void Message_Queue::send(Message & msg) {

	this->full->p();
	Message *sms = new Message(msg);
	this->_queue.insert(sms);
	this->empty->v();

}
Message_Queue::Message Message_Queue::receive() {

	this->empty->p();
	Message* msg =(Message*) this->_queue.remove();
	this->full->v();
	return *msg;

}
}
