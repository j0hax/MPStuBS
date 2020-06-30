// vim: set noet ts=4 sw=4:

#include "thread/dispatcher.h"

#include "debug/output.h"
#include "guard/guard.h"

Dispatcher dispatcher;

// https://www.sra.uni-hannover.de/Lehre/SS20/V_BSB/doc/classDispatcher.html#a1bd5ef08a829632075b6ccc7c81b1609
Dispatcher::Dispatcher() {
}

void Dispatcher::kickoff(Thread *t){
    DBG << "kickoff!" << endl;
    guard.leave();
    t->action();
}

void Dispatcher::go(Thread *first) {
    setActive(first);
    first->go();
}

void Dispatcher::dispatch(Thread *next) {
    DBG << "Dispatched " << next << endl;
    Thread* old = active();
    setActive(next);
    old->resume(next);
    
}

void Dispatcher::setActive(Thread *t){
    int curr_cpu = system.getCPUID();
	life[curr_cpu] = t;
}

Thread* Dispatcher::active() {
	int curr_cpu = system.getCPUID();
	return life[curr_cpu];
}