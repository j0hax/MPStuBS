// vim: set noet ts=4 sw=4:

#include "thread/scheduler.h"

void Scheduler::exit() {
    tl.lock();
    Thread* next = jobs.dequeue();
    tl.unlock();
    Dispatcher::dispatch(next);
}



void Scheduler::kill(Thread* that) {
    tl.lock();
    Thread* to_kill = jobs.remove(that);
    tl.unlock();

    // gucken, ob Thread in der Ready-Liste ist.
    if (to_kill == 0) {
        // Thread nicht in der ready-liste, soll vermerkt werden bei 
        that->set_kill_flag();
    }
}

void Scheduler::ready(Thread * that) {
    tl.lock();
    jobs.enqueue(that);
    tl.unlock();
}

void Scheduler::resume() {
    
    Thread* curr = Dispatcher::active();

    tl.lock();
    if (!curr->dying()) {
        jobs.enqueue(curr);
    }

    Thread* next = jobs.dequeue();
    tl.unlock();
    Dispatcher::dispatch(next);
}


void Scheduler::schedule() {
    tl.lock();
    Thread* next = jobs.dequeue();
    tl.unlock();
    Dispatcher::dispatch(next);
}