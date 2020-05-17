// vim: set noet ts=4 sw=4:

#include "device/panic.h"
#include "debug/output.h"
#include "machine/cpu.h"

Panic panic;

// not useful; gets ignored
Panic::Panic() {

    DBG_VERBOSE << "panic erstellt" << endl;

}

// default interrupt handler routine: sends message and freezes cpu
void Panic::trigger(){

    DBG << "INT: PANIC: CPU DIES!" << flush;
    CPU::die();

}
