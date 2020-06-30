// vim: set noet ts=4 sw=4:

#include "thread/scheduler.h"
#include "debug/output.h"
#include "machine/cpu.h"
#include "guard/guard.h"

Scheduler scheduler;


void Scheduler::exit() {
    guard.enter();
    Thread* next = jobs.dequeue();
    Dispatcher::dispatch(next);
}


void Scheduler::kill(Thread* that) {
    guard.enter();
    Thread* to_kill = jobs.remove(that);
    

    // gucken, ob Thread in der Ready-Liste ist.
    if (to_kill == 0) {
        // Thread nicht in der ready-liste, soll vermerkt werden bei 
        that->set_kill_flag();
    }
    guard.leave();
}


void Scheduler::ready(Thread * that) {
    jobs.enqueue(that);
}


void Scheduler::resume() {
    
    guard.enter();

    Thread* curr = Dispatcher::active();

    if (!curr->dying()) {
        jobs.enqueue(curr);
    }else{
        DBG << "tod!" << endl;
        CPU::die();
    }

    Thread* next = jobs.dequeue();
    if(next){
    Dispatcher::dispatch(next);
    }else{
        DBG << "queue null" << endl;
    }
    guard.leave();
    
}


void Scheduler::schedule() {
    Thread* first = jobs.dequeue();
    Dispatcher::go(first);
}
