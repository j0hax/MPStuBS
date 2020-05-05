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

extern APICSystem system;

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

extern "C" int main() {
  DBG.reset(' ', DBG.get_attribute());
  kout.reset(' ', kout.get_attribute());
  // Startmeldung ausgeben
  APICSystem::SystemType type = system.getSystemType();
  unsigned int numCPUs = system.getNumberOfCPUs();
  DBG_VERBOSE << "Is SMP system? " << (type == APICSystem::MP_APIC) << endl
              << "Number of CPUs: " << numCPUs << endl;

  switch (type) {
    case APICSystem::MP_APIC: {
      //Startet die AP-Prozessoren
      for (unsigned int i = 1; i < numCPUs; i++) {
        void* startup_stack = (void*) & (cpu_stack[(i) * CPU_STACK_SIZE]);
        DBG/*_VERBOSE*/ << "Booting CPU " << i << ", Stack: " << startup_stack << endl;
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

  kout << "1        <stream " << (int)'\n' << " result> -> <expected>" << endl;
  kout << "2:       " << true << " -> true" << endl;
  kout << "3:       " << 0 << " -> 0" << endl;
  kout << "4:        " << (10) << " -> 10" << endl;
  kout << "5 max:   " << ~((unsigned int)0) << " -> 4294967295" << endl;
  kout << "6 max:    " << ~(1 << 31) << " -> 2147483647" << endl;
  kout << "7 min:    " << (1 << 31) << " -> -2147483648" << endl;
  kout << "8 int:   " << (-123456789) << " -> -123456789" << endl;
  kout << "9 int:   " << (123456789) << " -> 123456789" << endl;
  kout << "\t10:     " << bin << 42 << dec << " -> 0b\t101010" << endl;
  kout << "11:      " << oct << 42 << dec << " -> 052" << endl;
  kout << "12:        " << hex << 42 << dec << " -> 0x2a" << endl;
  kout << "13:    " << ((void*)(3735928559u)) << " -> 0xdeadbeef" << endl;
  kout << "14:     " << ((char)3) << endl;    // a heart
  kout << "15        <stream " << (int)'\n' << " result> -> <expected>" << endl;
  kout << "16:       " << true << " -> true" << endl;
  kout << "17:       " << 0 << " -> 0" << endl;
  kout << "18:        " << (10) << " -> 10d" << endl;
  kout << "19: max:   " << ~((unsigned int)0) << " -> 4294967295 ok" <<
       " Teste die Ausgabe: Zeilenumbruch automatisch einfuegen... " << endl;
  kout << "20: \thello: now a tab:\tand now some text..." << flush;
  // Instantiate and echo the keyboard
  Keyboard_Controller kctrl;
  Key in;

  while (true) {
    in = kctrl.key_hit();

    if (in.valid()) {
      kout << in << flush;
    }

    if (in.ctrl() && in == 'c') {
      kctrl.reboot();
    }
  }

  return 0;
}

/*! \brief Einsprungpunkt für Applikationsprozessoren
 *
 *  Code in dieser Funktion wird auf allen Applikationsprozessoren ausgeführt
 *  (entspricht allen CPUs außer derjenigen mit der ID 0).
 */
extern "C" int main_ap() {
  DBG.reset(' ', DBG.get_attribute());
  DBG_VERBOSE << "CPU " << (int) system.getCPUID()
              << "/LAPIC " << (int) lapic.getLAPICID() << " in main_ap()" << endl;
  return 0;
}
