// vim: set noet ts=4 sw=4:

#include "debug/output.h"
#include "device/keyboard.h"
#include "machine/plugbox.h"

extern CGA_Stream kout;

// global instance
Keyboard keyboard;

// not really useful but gets ignored anyway
Keyboard::Keyboard(){
    DBG_VERBOSE << "keyboard created" << endl;
}

// plugs in keyboard
void Keyboard::plugin(){

    // assigning this (adr of global obj) as a keyboard interrupt handler to plugbox
    plugbox.assign(Plugbox::Vector::keyboard, this);

}

void Keyboard::trigger(){

    //DBG << "KEYBRD" << endl;
    Key in = key_hit();
    if(in.valid()){
        if (in.ctrl() && in.scancode() == Key::scan::del ) {
            reboot();
        }
        // debugging
        DBG << "INT: KBD: " << in << endl;
        kout << in << flush;
    }

}
