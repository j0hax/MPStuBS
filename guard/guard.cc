// vim: set noet ts=4 sw=4:

#include "guard/guard.h"
#include "debug/output.h"


#include "machine/lapic.h"
#include "machine/cpu.h"

#include "secure.h"

extern APICSystem system;

// kleine Variable um zu gucken ob wir in Ebene 1/2 sind
volatile bool flag[4];

// Warten bis Ebene 1/2 verfügbar ist
void Guard::enter() {
  s_lock.lock();
  flag[system.getCPUID()] = true;
}

// Die Epiloge abarbeiten
void Guard::leave() {
  int curr_cpu = system.getCPUID();
  Gate* item;
  // IRQs enabled
  while (true) {
    CPU::disable_int();
    item = queues[curr_cpu].dequeue();
    if (item == 0) break;
    CPU::enable_int();
    item->epilogue();
    item->set_dequeued();
  }

  CPU::enable_int();

  flag[curr_cpu] = false;
  s_lock.unlock();

  // Zurück zur Ebene 0 (normaler Programmbetrieb)
}

void Guard::relay(Gate* item) {
  int curr_cpu = system.getCPUID();
  // E1 -> E1/2
  if (flag[curr_cpu]) {
    queues[curr_cpu].enqueue(item);
    DBG << "added to queue." << endl;
  } else {
    {
      Secure section;
      CPU::disable_int();
      item->epilogue();
      item->set_dequeued();
      CPU::enable_int();
      DBG << "processed." << endl;
    };
  }
}
