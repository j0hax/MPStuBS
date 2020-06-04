// vim: set noet ts=4 sw=4:

#include "guard/guard.h"
#include "debug/output.h"

extern APICSystem system;

void Guard::enter(){

    DBG << __func__ << flush;
    s_lock.lock();
    

}

void Guard::leave(){
    DBG << __func__ << endl;
    s_lock.unlock();
}

void Guard::relay(Gate *item){
    enter();
    int curr_cpu = system.getCPUID();
    queues[curr_cpu].enqueue(item);
    leave();
}
