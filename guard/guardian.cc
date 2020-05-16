// vim: set noet ts=4 sw=4:

/*! \file
 *  \brief Low-Level Interrupt-Behandlung
 */
#include "types.h"
#include "guardian.h"

#include "machine/cpu.h"
#include "../machine/lapic.h"
#include "../machine/cpu.h"
#include "../debug/output.h"

extern CGA_Stream kout;

extern "C" void guardian(uint32_t vector, irq_context* context) {

  (void) vector;
  (void) context;
  //kout << "interrupt in guardian" << flush;
  DBG << "guardian: " << vector << ' ' << context << endl;

  lapic.ackIRQ();
}
