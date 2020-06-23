// vim: set noet ts=4 sw=4:

#include "thread/dispatcher.h"

#include "debug/output.h"

void Dispatcher::kickoff(Thread *t){

    DBG << "kickoff!" << flush;
    t->action();

}
