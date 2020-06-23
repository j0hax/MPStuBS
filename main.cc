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

extern APICSystem system;
extern IOAPIC ioapic;
Spinlock spinlock;
Ticketlock ticketlock;


static const unsigned long CPU_STACK_SIZE = 4096;
// Stack fuer max. 7 APs
static unsigned char cpu_stack[(CPU_MAX - 1) * CPU_STACK_SIZE];

/*! \brief Einsprungpunkt ins System
 *
 *  Dieser Code wird nur auf der Boot-CPU (diejenige mit der ID 0) ausgeführt.
 */

/*struct Attribute {
	char fg:4;
	char bg:3;
	bool blink;
}__attribute__((packed));
*/
CGA_Screen::Attribute k0(CGA_Screen::color::LIGHT_GREY,
                         CGA_Screen::color::BLACK, false);
CGA_Screen::Attribute c0(CGA_Screen::color::BLACK, CGA_Screen::color::BROWN,
                         false);
CGA_Screen::Attribute c1(CGA_Screen::color::LIGHT_GREY, CGA_Screen::color::BLUE,
                         false);
//CGA_Screen::Attribute c2(CGA_Screen::color::BLACK, CGA_Screen::color::CYAN, false);
//CGA_Screen::Attribute c3(CGA_Screen::color::BLACK, CGA_Screen::color::BROWN, false);

CGA_Stream kout(0, 79, 0, 16, true, k0);
CGA_Stream dout_CPU0(0, 38, 17, 20, false, c0);
CGA_Stream dout_CPU1(39, 79, 17, 20, false, c1);
CGA_Stream dout_CPU2(0, 38, 21, 24, false, c1);
CGA_Stream dout_CPU3(39, 79, 21, 24, false, c0);

// global instance
Keyboard keyboard;

//init guard
Guard guard;

volatile int i = 0;

extern "C" int main() {
  // enable interrupts for this core
  CPU::enable_int();

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
  
  a1.go();
  
  /*ticketlock.lock();
  Application app1(i++);
  ticketlock.unlock();
  app1.action();*/

  /*
  kout << "0Test        <stream result> -> <expected>" << endl;
  kout << "1bool:       " << true << " -> true" << endl;
  kout << "2zero:       " << 0 << " -> 0" << endl;
  kout << "3ten:        " << (10) << " -> 10" << endl;
  kout << "4uint max:   " << ~((unsigned int)0) << " -> 4294967295" << endl;
  kout << "5int max:    " << ~(1<<31) << " -> 2147483647" << endl;
  kout << "6int min:    " << (1<<31) << " -> -2147483648" << endl;
  kout << "7some int:   " << (-123456789) << " -> -123456789" << endl;
  kout << "8some int:   " << (123456789) << " -> 123456789" << endl;
  kout << "9binary:     " << bin << 42 << dec << " -> 0b101010" << endl;
  kout << "10octal:     " << oct << 42 << dec << " -> 052" << endl;
  kout << "11hex:       " << hex << 42 << dec << " -> 0x2a" << endl;
  kout << "12pointer:   " << ((void*)(3735928559u)) << " -> 0xdeadbeef" << endl;
  kout << "13smiley:    " << ((char)1) << endl;    // a heart
  kout << "tabs:\t1\t1\t\t4" << endl;
  */

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
  DBG << "doing nothing" << flush;
  for (;;);

  return 0;
}
