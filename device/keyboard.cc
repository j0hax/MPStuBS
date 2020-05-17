// vim: set noet ts=4 sw=4:

#include "debug/output.h"
#include "device/keyboard.h"
#include "machine/plugbox.h"

extern CGA_Stream kout;

Keyboard keyboard;

Keyboard::Keyboard(){
    DBG_VERBOSE << "keyboard created" << endl;
}

void Keyboard::plugin(){

    plugbox.assign(Plugbox::Vector::keyboard, this);

}

void Keyboard::trigger(){

    //DBG << "KEYBRD" << endl;
    Key in = key_hit();
    if(in.valid()){
        if (in.ctrl() && in.scancode() == Key::scan::del ) {
            reboot();
        }
        DBG << in << flush;
        kout << in << flush;
    }

}
