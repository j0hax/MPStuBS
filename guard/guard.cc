// vim: set noet ts=4 sw=4:

#include "guard/guard.h"
#include "debug/output.h"

extern APICSystem system;

// In Ebene 1/2 absetzen
void Guard::enter(){
    s_lock.lock();
}

// Abarbeiten oder in Ebene 0 versetzen
void Guard::leave(){
    s_lock.unlock();
    // TODO: weitere epiloge abarbeiten
}

void Guard::relay(Gate *item){
    enter();
    int curr_cpu = system.getCPUID();
    queues[curr_cpu].enqueue(item);
    DBG << "Relayed gate " << item << " to CPU " << curr_cpu << endl;
    leave();
}
