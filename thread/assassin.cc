// vim: set noet ts=4 sw=4:

#include "thread/assassin.h"


void Assassin::epilogue() {

}

void Assassin::hire() {
	plugbox.assign(Plugbox::Vector::assassin, this);
}


bool Assassin::prologue() {
    return true;
}