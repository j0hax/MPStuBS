// vim: set noet ts=4 sw=4:

#include "thread/thread.h"
#include "thread/dispatcher.h"

Thread::Thread(void* tos){

    toc_settle(&regs, tos, &Dispatcher::kickoff, this);

}

void Thread::go(){

    toc_go(&regs);

}

void Thread::resume(Thread *next){

    toc_switch(&regs, &(next->regs));

}
