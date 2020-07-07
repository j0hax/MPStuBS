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
#include "device/watch.h"

#include "syscall/guarded_scheduler.h"


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

Watch watch;

volatile int i = 0;

extern "C" int main() {
  // enable interrupts for this core
  //CPU::enable_int();

  CPU::disable_int();

  //UNCOMMENT FOR INTERRUPTS

  // init ioapic (global instance)
  ioapic.init();
  // redirect keyboard interrupt
  uint32_t kbd_slot = system.getIOAPICSlot(APICSystem::Device::keyboard);
  ioapic.config(kbd_slot, Plugbox::Vector::keyboard);
  // unmask keyboard interrupt
  ioapic.allow(kbd_slot);
  // plug keyboard in plugbox (as interrupt handler for the keyboard)
  keyboard.plugin();

  
  //tests: 1.000.000 us = 1sec
  //watch.windup(1000000);
  watch.windup(50000);
  DBG << "lapic interval:" << watch.interval()  << "us"<< endl;
  

  // clear screen
  kout.reset(' ', kout.get_attribute());
  // Startmeldung ausgeben
  APICSystem::SystemType type = system.getSystemType();
  unsigned int numCPUs = system.getNumberOfCPUs();


  Guarded_Scheduler::ready(&a1);
  Guarded_Scheduler::ready(&a2);
  Guarded_Scheduler::ready(&a3);
  Guarded_Scheduler::ready(&a4);
  Guarded_Scheduler::ready(&a5);
  Guarded_Scheduler::ready(&a6);
  
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

  ticketlock.lock();
  i++;
  ticketlock.unlock();
  while(i != 4){}

  watch.activate();
  
  {
    Secure s;
    //watch.activate();
    scheduler.schedule();
  }

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

  ticketlock.lock();
  i++;
  ticketlock.unlock();
  while(i != 4){}

  watch.activate();
  {
    Secure s;
    scheduler.schedule();
  }

  DBG << "doing nothing" << flush;
  for (;;);

  return 0;
}
