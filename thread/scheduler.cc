// vim: set noet ts=4 sw=4:

#include "thread/scheduler.h"
#include "debug/output.h"
#include "machine/cpu.h"
#include "guard/guard.h"
#include "device/watch.h"


#include "machine/apicsystem.h"

Scheduler scheduler;


void Scheduler::exit() {
    //guard.enter();
    Thread* next = jobs.dequeue();
    Dispatcher::dispatch(next);
}


void Scheduler::kill(Thread* that) {
    // Da Interrupts teuer sind, senden wir den Interrupt nur an der CPU mit dem Thread.

    // kill flag setzen
    /*
    Fall 1: set_kill_flag -> IPI -> wechselt Thread -> alter Thread wird nicht enqueued
    Fall 2: set_kill_flag -> WatchINT -> wechselt Thread -> alter Thread wird nicht enqueued
    Fall 3: Thread wechselt -> set_kill_flag -> deqeue bzw erneutes enqueue wird verhindert
    */
    that->set_kill_flag();

	// Check Queues
	if (jobs.remove(that) != 0) {
		return;
	}

	// Check if running on a core
    Thread** running = Dispatcher::life;

    int limit = system.getNumberOfOnlineCPUs();
    int target = 0;
    for (; target < limit; target++) {
        if (that == running[target]) break;
    }
    // convert to binary
    unsigned char dest = 1 << target;
    // send the command
    system.sendCustomIPI(dest, Plugbox::Vector::assassin);
}


void Scheduler::ready(Thread * that) {
    jobs.enqueue(that);
}


void Scheduler::resume() {
    
    //guard.enter();
    Thread* curr = Dispatcher::active();
    if(!curr) {
        DBG << "active is null" << endl;
        CPU::die();
    }

    if (!curr->dying()) {
        jobs.enqueue(curr);
    }else{
        DBG << "tod!" << endl;
        //CPU::die();
    }

    // dequeue threads until one is found that is not dying
    Thread* next = jobs.dequeue();
    while(next && next->dying()) next = jobs.dequeue();

    if(next){
    Dispatcher::dispatch(next);
    }else{
        DBG << "queue null" << endl;
        CPU::die();
    }
    //guard.leave();
    
}


void Scheduler::schedule() {
    Thread* first = jobs.dequeue();
    if(!first) {
        DBG << "no job to start" << endl;
        CPU::die();
    }
    watch.activate();
    Dispatcher::go(first);
}
