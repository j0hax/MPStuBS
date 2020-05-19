// vim: set noet ts=4 sw=4:

#include "device/cgastr.h"
extern CGA_Stream kout;

/*! \file
 *  \brief Enthält die Klasse Application
 */

#pragma once

/*! \brief Die Klasse Application definiert eine Anwendung für OO-Stubs.
 */
class Application {
  // Verhindere Kopien und Zuweisungen
  Application(const Application &)            = delete;
  Application &operator=(const Application &) = delete;

public:
  /*! \brief Konstruktor
   * 
   * \param i Instanz-ID
   */

    int instanceID;

    Application(int i) {
      instanceID = i;
      kout << "Launched app1 with InstanceID " << instanceID << endl;
    }

  /*! \brief Enthält den Code der Anwendung
   *
   */
  void action();
};

