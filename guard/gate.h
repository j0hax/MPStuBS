// vim: set noet ts=4 sw=4:

/*! \file
 *  \brief Enthält die Klasse Gate
 */

#pragma once

#include "machine/spinlock.h"
#include "object/queuelink.h"

/*! \brief Klasse von Objekten, die in der Lage sind, Unterbrechungen zu behandeln.
 *  \ingroup interrupts
 *
 *
 * Die Klassen aller Objekte, die in der Plugbox eingetragen werden sollen,
 * müssen von Gate abgeleitet werden.
 *
 * Alle diese Objekte müssen die virtuelle Methode trigger() definieren.
 */
class Gate {
  // Verhindere Kopien und Zuweisungen
  Gate(const Gate &) = delete;
  Gate &operator=(const Gate &) = delete;
private:
  Spinlock gate_lock;
  volatile bool queue_flag = false;
public:

  QueueLink<Gate> queue_link;

  /*! \brief Konstruktor
   *
   */
  Gate() {}
  /*! \brief Destruktor
   *
   *  Klassen mit virtuellen Methoden sollten grundsätzlich einen virtuellen
   *  Destruktor besitzen, der auch leer sein darf. Bei OO-Stubs beruhigt
   *  das den Compiler, bei anderen Systemen garantiert das, dass die
   *  Speicherfreigabe mit delete für Objekte der abgeleiteten Klassen
   *  korrekt funktioniert.
   */
  virtual ~Gate() {}
  /*! \brief Unterbrechungsbehandlungsroutine
   *
   *  Diese Methode wird sofort nach Auftreten der Unterbrechung asynchron zu
   *  anderen Kernaktivitäten ausgeführt. Da sie als rein virtuelle Methode
   *  implementiert ist, muss sie durch die abgeleiteten Klassen spezifiziert
   *  werden.
   */
  //virtual void trigger() = 0;

  virtual void epilogue() {}
  virtual bool prologue() {
    return false;
  }

  void set_dequeued() {
    gate_lock.lock();
    queue_flag = false;
    gate_lock.unlock();
  }

  bool set_queued() {
    bool status;
    gate_lock.lock();
    status = !queue_flag;
    queue_flag = true;
    gate_lock.unlock();
    return status;
  }

};
