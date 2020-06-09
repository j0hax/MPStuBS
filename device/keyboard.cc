// vim: set noet ts=4 sw=4:

#include "debug/output.h"
#include "device/keyboard.h"
#include "machine/plugbox.h"

extern CGA_Stream kout;

Keyboard::Keyboard() {
  DBG_VERBOSE << "keyboard created" << endl;
  kout.getpos(pos_x, pos_y);
}

// plugs in keyboard
void Keyboard::plugin() {
  // assigning this (adr of global obj) as a keyboard interrupt handler to plugbox
  plugbox.assign(Plugbox::Vector::keyboard, this);
}

void Keyboard::trigger() {
  //DBG << "KEYBRD" << endl;
  Key in = key_hit();

  if (in.valid()) {
    if (in.ctrl() && in.scancode() == Key::scan::del) {
      reboot();
    }

    // debugging
    DBG_VERBOSE << "INT: KBD: " << in << endl;
    //int x, y;
    //ticketlock.lock();
    //kout.getpos(x, y);
    kout.setpos(pos_x, pos_y);
    kout << in << flush;
    kout.getpos(pos_x, pos_y);
    //kout.setpos(x, y);
    //ticketlock.unlock();
  }
}

bool Keyboard::prologue() {
  // say that an epilogue is needed
  return true;
}

void Keyboard::epilogue() {
  trigger();
}
