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

	// TODO: Protect with secure or guard?

    // Gucken wo der Interrupt ist

	// Check Queues
	if (jobs.remove(that) != 0) {
		that->set_kill_flag();
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

    Thread* next = jobs.dequeue();

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
