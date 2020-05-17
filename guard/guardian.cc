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

extern "C" void guardian(uint32_t vector, irq_context* context) {

  //kout << "interrupt in guardian" << flush;
  
  // getting a gate pointer (interrupt handler) to the corresponding vector
  // plugbox stores all 256 interrupt vectors (VDT)
  Gate* ir_handler = plugbox.report(vector);
  // triggering the interrupt handler (by using gate interface)
  ir_handler->trigger();

  lapic.ackIRQ();
}
