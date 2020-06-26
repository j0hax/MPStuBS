// vim: set noet ts=4 sw=4:

#include "device/cgastr.h"
#include "debug/output.h"
#include "machine/cpu.h"
#include "machine/spinlock.h"
#include "machine/ticketlock.h"
#include "guard/secure.h"
#include "thread/dispatcher.h"

extern CGA_Stream kout;
extern Spinlock spinlock;
extern Ticketlock ticketlock;


/*! \file
 *  \brief Enthält die Klasse Application
 */

#pragma once

/*! \brief Die Klasse Application definiert eine Anwendung für OO-Stubs.
 */
class Application : public Thread {
  // Verhindere Kopien und Zuweisungen
  Application(const Application &)            = delete;
  Application &operator=(const Application &) = delete;

public:
  /*! \brief Konstruktor
   *
   * \param i Instanz-ID
   */

  int instanceID;

  Application(int i, void* tos) : Thread(tos), instanceID(i) {};


  /*! \brief Enthält den Code der Anwendung
   *
   */
  void action();
};

extern Application a1;
extern Application a2;

