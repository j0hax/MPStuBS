// vim: set noet ts=4 sw=4:

#include "thread/assassin.h"
#include "machine/cpu.h"
#include "debug/output.h"

Assassin assassin;

void Assassin::epilogue() {
	// check if thread is dying
	Thread* current = scheduler.active();
	if (current->dying()) {
		scheduler.resume();
	}
}

void Assassin::hire() {
	plugbox.assign(Plugbox::Vector::assassin, this);
}


bool Assassin::prologue() {
    return true;
}