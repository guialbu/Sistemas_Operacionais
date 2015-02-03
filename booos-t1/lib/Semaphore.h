/*
 * Semaphore.h
 *
 *  Created on: 4 Dec 2014
 *      Author: heisenberg
 */

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include <BOOOS.h>
#include <Queue.h>

namespace BOOOS
{

class Semaphore
{
public:
    Semaphore(int i = 1);
    virtual ~Semaphore();

    void p();
    void v();

private:
    volatile int sem;
    Queue _waiting;
};

} /* namespace BOOOS */

#endif /* SEMAPHORE_H_ */

