// vim: set noet ts=4 sw=4:

/*! \file
 *  \brief Low-Level Interrupt-Behandlung
 */
#include "types.h"
#include "guardian.h"

#include "machine/cpu.h"
#include "machine/lapic.h"
#include "machine/cpu.h"
#include "debug/output.h"
#include "machine/plugbox.h"
#include "guard.h"

extern "C" void guardian(uint32_t vector, irq_context* context) {

  // context not important for now or?
  (void) context;
  // getting a gate pointer (interrupt handler) to the corresponding vector
  // plugbox stores all 256 interrupt vectors (VDT)
  Gate* ir_handler = plugbox.report(vector);
  // triggering the interrupt handler (by using gate interface)
  //ir_handler->trigger();

  /* TODO: Interrupts only working on one CPU */

  /*if (ir_handler->prologue()) {
    DBG << "Gate " << vector << ": " << flush;
    if (ir_handler->set_queued() == false) {
      DBG << "already in queue." << endl;
    } else {
      ir_handler->set_dequeued();
      // Enqueue or process the gate
      DBG << "relay: " << flush;
      guard.relay(ir_handler);
    }
  } else {
    DBG << "Interrupt has no epilogue." << endl;
  }*/
  DBG << "int" << endl;
  guard.relay(ir_handler);
  //lapic.ackIRQ();

}
