// vim: set noet ts=4 sw=4:

/*! \file
 *  \brief Enthält mit den main() und main_ap() Funktionen den Startpunkt für
 *  das System
 */

/* INCLUDES */
#include "machine/apicsystem.h"
#include "machine/lapic.h"
#include "debug/output.h"
#include "machine/keyctrl.h"
#include "machine/ioapic.h"
#include "machine/cpu.h"
#include "device/panic.h"
#include "device/keyboard.h"

#include "user/app1/appl.h"

#include "machine/spinlock.h"
#include "machine/ticketlock.h"

#include "guard/guard.h"
#include "guard/secure.h"

#include "machine/toc.h"

#include "thread/scheduler.h"

Spinlock spinlock;
Ticketlock ticketlock;


static const unsigned long CPU_STACK_SIZE = 4096;
// Stack fuer max. 7 APs
static unsigned char cpu_stack[(CPU_MAX - 1) * CPU_STACK_SIZE];

/*! \brief Einsprungpunkt ins System
 *
 *  Dieser Code wird nur auf der Boot-CPU (diejenige mit der ID 0) ausgeführt.
 */

// global instance
Keyboard keyboard;

//init guard
Guard guard;

volatile int i = 0;

extern "C" int main() {
  // enable interrupts for this core
  //CPU::enable_int();

  CPU::disable_int();

  /* //UNCOMMENT FOR INTERRUPTS

  // init ioapic (global instance)
  ioapic.init();
  // redirect keyboard interrupt
  uint32_t kbd_slot = system.getIOAPICSlot(APICSystem::Device::keyboard);
  ioapic.config(kbd_slot, Plugbox::Vector::keyboard);
  // unmask keyboard interrupt
  ioapic.allow(kbd_slot);
  // plug keyboard in plugbox (as interrupt handler for the keyboard)
  keyboard.plugin();
  
  */
  
  // clear screen
  kout.reset(' ', kout.get_attribute());
  // Startmeldung ausgeben
  APICSystem::SystemType type = system.getSystemType();
  unsigned int numCPUs = system.getNumberOfCPUs();


  scheduler.ready(&a1);
  scheduler.ready(&a2);
  scheduler.ready(&a3);
  scheduler.ready(&a4);
  scheduler.ready(&a5);
  scheduler.ready(&a6);
  
  // old way of switching threads
  //dispatcher.go(&a1);


  DBG/*_VERBOSE*/ << "CPU " << (int) system.getCPUID()
                  << "/LAPIC " << (int) lapic.getLAPICID() << " in main()" << endl;
  DBG_VERBOSE << "Is SMP system? " << (type == APICSystem::MP_APIC) << endl
              << "Number of CPUs: " << numCPUs << endl;

  switch (type) {
    case APICSystem::MP_APIC: {
      //Startet die AP-Prozessoren
      for (unsigned int i = 1; i < numCPUs; i++) {
        void* startup_stack = (void*) & (cpu_stack[(i) * CPU_STACK_SIZE]);
        DBG_VERBOSE << "Booting CPU " << i << ", Stack: " << startup_stack << endl;
        system.bootCPU(i, startup_stack);
      }

      break;
    }

    case APICSystem::UP_APIC: {
      break;
    }

    case APICSystem::UNDETECTED: {
    }
  }

  scheduler.schedule();


  /*ticketlock.lock();
  Application app1(i++);
  ticketlock.unlock();
  app1.action();*/

  // main loop
  for (;;);

  return 0;
}

/*! \brief Einsprungpunkt für Applikationsprozessoren
 *
 *  Code in dieser Funktion wird auf allen Applikationsprozessoren ausgeführt
 *  (entspricht allen CPUs außer derjenigen mit der ID 0).
 */
extern "C" int main_ap() {
  //DBG.reset(' ', DBG.get_attribute());
  DBG/*_VERBOSE*/ << "CPU " << (int) system.getCPUID()
                  << "/LAPIC " << (int) lapic.getLAPICID() << " in main_ap()" << endl;
  //main_ap loop
  /*
  CPU::enable_int();
  ticketlock.lock();
  Application app_ap(i++);
  ticketlock.unlock();
  app_ap.action();
  */
  CPU::disable_int();
  scheduler.schedule();

  DBG << "doing nothing" << flush;
  for (;;);

  return 0;
}
