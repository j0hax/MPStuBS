// vim: set noet ts=4 sw=4:

#include "device/panic.h"
#include "debug/output.h"
#include "machine/cpu.h"

Panic panic;

Panic::Panic() {

    DBG_VERBOSE << "panic erstellt" << endl;

}

void Panic::trigger(){

    DBG << "PANIC: CPU DIES!" << flush;
    CPU::die();

}
