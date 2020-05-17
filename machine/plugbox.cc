// vim: set noet ts=4 sw=4:

/* INCLUDES */

#include "machine/plugbox.h"
#include "debug/output.h"
#include "device/panic.h"

extern Panic panic;

Plugbox plugbox;

Plugbox::Plugbox(){

    DBG_VERBOSE << "plugbox created" << endl;

    for(int i = 0; i < 256; i++){
        gateobjs[i] = &panic;
    }

}

void Plugbox::assign(unsigned int vector, Gate *gate){

    if(vector <= 255)
    gateobjs[vector] = gate;

}

Gate* Plugbox::report(unsigned int vector){

    if(vector <= 255)
    return gateobjs[vector];
    return NULL;
}
