// vim: set noet ts=4 sw=4:
/*! \file
 *  \brief Enthält die Klasse Spinlock
 */

#pragma once

/*! \brief Mit Hilfe eines Spinlocks kann man Codeabschnitte serialisieren, die
 *  echt nebenläufig auf mehreren CPUs laufen.
 *  \ingroup sync
 *
 *
 *  Die Synchronisation läuft dabei über eine Sperrvariable. Sobald jemand den
 *  kritischen Abschnitt betreten will, setzt er die Sperrvariable. Verlässt
 *  er den kritischen Abschnitt, so setzt er sie wieder zurück. Ist die
 *  Sperrvariable beim Versuch des Betretens jedoch schon gesetzt, dann wartet
 *  der Aufrufer aktiv darauf, dass sie der Besitzer des kritischen Abschnittes
 *  beim Verlassen wieder zurücksetzt.
 *
 *  Zur Implementierung können die beiden GCC-Intrinsics
 *  \c __sync_lock_test_and_set(unsigned int* lock_status, unsigned int value)
 *  und \c __sync_lock_release(unsigned int* lock_status) verwendet werden.
 *  Diese werden vom Compiler in die architekturspezifischen atomaren
 *  Operationen übersetzt.
 *
 *  <a href="http://gcc.gnu.org/onlinedocs/gcc-4.1.2/gcc/Atomic-Builtins.html">Eintrag im GCC Manual über Atomic Builtins</a>
 */
class Spinlock {
private:
  Spinlock(const Spinlock &copy); //verhindert Kopieren

  volatile unsigned int _lock = 0;

public:
  /*! \brief Konstruktor; Initialisierung des Spinlocks als ungesperrt.
   *
   *  \todo Konstruktor vervollständigen
   *
   */
  Spinlock() {
    unlock();
  }
  /*! \brief Betritt den gesperrten Abschnitt. Ist dieser besetzt, so wird
   *  solange aktiv gewartet, bis er betreten werden kann.
   *
   *  \todo Methode implementieren
   */
  void lock() {
    while (__sync_lock_test_and_set(&_lock, 1));
  }
  /*! \brief Gibt den gesperrten Abschnitt wieder frei.
   *
   *
   *  \todo Methode implementieren
   */
  void unlock() {
    __sync_lock_release(&_lock);
  }
};

