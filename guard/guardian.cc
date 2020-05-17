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

  (void) vector;
  (void) context;
  //kout << "interrupt in guardian" << flush;
  DBG << "interrupt " << flush;

  Gate* ir_handler = plugbox.report(vector);
  ir_handler->trigger();

  DBG << endl;

  lapic.ackIRQ();
}
