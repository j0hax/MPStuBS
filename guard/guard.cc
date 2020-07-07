// vim: set noet ts=4 sw=4:

#include "guard/guard.h"
#include "debug/output.h"


#include "machine/lapic.h"
#include "machine/cpu.h"

#include "secure.h"
#include "syscall/guarded_scheduler.h"


extern APICSystem system;

// Warten bis Ebene 1/2 verfügbar ist
void Guard::enter() {
  
  CPU::disable_int();
  flag[system.getCPUID()] = true;
  CPU::enable_int();
  s_lock.lock();
  //CPU::enable_int();
  //DBG << "enter" << flush;
  //flag[system.getCPUID()] = true;

}

// Die Epiloge abarbeiten
void Guard::leave() {
  Gate* item;
  // IRQs enabled
  while (true) {
    CPU::disable_int();

    item = queues[system.getCPUID()].dequeue();
    if (!item) {
      //DBG << "break" << flush;
      break;
    }
    item->set_dequeued();
    DBG << " dequ " << endl;
    CPU::enable_int();
    item->epilogue();
    DBG << "processed2." << endl;

  }                                          

  flag[system.getCPUID()] = false;
  s_lock.unlock();
  CPU::enable_int();
  
  // Zurück zur Ebene 0 (normaler Programmbetrieb)
}

void Guard::relay(Gate* item) {
  //CPU::disable_int();
  
  if (item->prologue()) {
    lapic.ackIRQ();

    int curr_cpu = system.getCPUID();
    // E1 -> E1/2
    if (flag[curr_cpu]) {

      if (item->set_queued() == false) {
        DBG << "already in queue." << endl;
      }else{
        queues[curr_cpu].enqueue(item);
        //DBG << item->set_queued();
        DBG << " enqu " << endl;
      }

    } else {
      CPU::enable_int();

      enter();
      item->epilogue();
      DBG << "processed1." << endl;
      leave();

      return;
    }
  }
  lapic.ackIRQ();
  CPU::enable_int();

}

// problem enqu aber nie wieder dequeue oder processed
// 1. interrupts an
// 2. flag[curr_cpu] == true
// 3. nach while-leave 
