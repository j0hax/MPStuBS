// vim: set noet ts=4 sw=4:

#include "guard/guard.h"
#include "debug/output.h"

void Guard::enter(){
    DBG << "enter" << flush;
}

void Guard::leave(){
    DBG << "leave" << endl;
}

void Guard::relay(Gate *item){

    (void)item;

}
