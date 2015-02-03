/*
 * MessageQueue.h
 *
 *
 *      Author: ana e karol
 */

#ifndef MESSAGE_QUEUE_H_
#define MESSAGE_QUEUE_H_

#include <BOOOS.h>
#include <Queue.h>
#include <string.h>
#include <Semaphore.h>

namespace BOOOS{

class Message_Queue{
public:

    class Message: public Queue::Element
    {
    public:
        static const int MSG_SIZE = BOOOS::MESSAGE_SIZE;

        Message(char msg[MSG_SIZE]) { memcpy(_msg, msg, MSG_SIZE); }
        virtual ~Message() {}

        char * get() { return _msg; }

    private:
        char _msg[MSG_SIZE];
    };

    Message_Queue(int max_size = 5);
    virtual ~Message_Queue();

    void send(Message & msg);
    Message receive();

    int count() { return _queue.length(); }

private:
    Queue _queue;
    int _max_size;
    Semaphore * full;
    Semaphore * empty;

};

} /* namespace BOOOS */

#endif /* MESSAGEQUEUE_H_ */
