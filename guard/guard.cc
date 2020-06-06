// vim: set noet ts=4 sw=4:

#include "guard/guard.h"
#include "debug/output.h"

extern APICSystem system;

// Warten bis Ebene 1/2 verfügbar ist
void Guard::enter() {
  s_lock.lock();
}

// Abarbeiten oder zurück in Ebene 0 versetzen
void Guard::leave() {
  // TODO: weitere epiloge abarbeiten
  int curr_cpu = system.getCPUID();
  Gate* item;

  while ((item = queues[curr_cpu].dequeue())) {
    item->epilogue();
    item->set_dequeued();
  }

  s_lock.unlock();
}

void Guard::relay(Gate* item) {
  enter();
  int curr_cpu = system.getCPUID();
  queues[curr_cpu].enqueue(item);
  DBG << "Relayed gate " << item << " to CPU " << curr_cpu << endl;
  leave();
}
