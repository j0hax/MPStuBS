// vim: set noet ts=4 sw=4:

#include "guard/guard.h"
#include "debug/output.h"


#include "machine/lapic.h"
#include "machine/cpu.h"

#include "secure.h"

extern APICSystem system;

// Warten bis Ebene 1/2 verfügbar ist
void Guard::enter() {
  
  CPU::disable_int();
  s_lock.lock();
  flag = true;
  CPU::enable_int();
  //DBG << "enter" << flush;
  //flag[system.getCPUID()] = true;

}

// Die Epiloge abarbeiten
void Guard::leave() {
  int curr_cpu = system.getCPUID();
  Gate* item;
  // IRQs enabled
  while (true) {
    CPU::disable_int();

    item = queues[curr_cpu].dequeue();
    if (!item) break;
    item->set_dequeued();
    DBG << " dequ " << endl;
    CPU::enable_int();
    item->epilogue();
  }

  //flag[curr_cpu] = false;
  flag = false;
  s_lock.unlock();
  CPU::enable_int();
  // Zurück zur Ebene 0 (normaler Programmbetrieb)
}

void Guard::relay(Gate* item) {
  int curr_cpu = system.getCPUID();
  // E1 -> E1/2
  //if (flag[curr_cpu]) {
  if (flag) {
    queues[curr_cpu].enqueue(item);
    DBG << item->set_queued();
    DBG << " enqu " << endl;
  } else {
    CPU::enable_int();
    lapic.ackIRQ();
    {
      Secure section;
      //CPU::disable_int();
      
      item->epilogue();
      
      //CPU::enable_int();
      DBG << "processed." << endl;

    }

  }
}
