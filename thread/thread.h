// vim: set noet ts=4 sw=4:

/*! \file
 *  \brief Enthält die Klasse Thread
 */

/*! \defgroup thread Multithreading
 *  \brief Das Multithreading Subsystem
 *
 * Die Multithreading Gruppe enthält alle Elemente die Grundlegend für
 * das CPU Multiplexing benötigt werden. Ziel dieses Modules ist es
 * die Abstraktion des Threads an zu bieten, die für die Anwendung
 * eine virtualisierte CPU bereit stellt.
 */

#pragma once

#include "machine/toc.h"

#include "object/queuelink.h"

/*! \brief Der Thread ist das Objekt der Ablaufplanung.
 *  \ingroup thread
 */
class Thread
{
public:
	/*! \brief Konstruktor.
	 *
	 *  Initialisiert den Kontext mit Hilfe von toc_settle
	 *  \param tos Top of Stack, also die höchste Adresse des Speicherbereiches,
	 *  der als Stack für diesen Thread fungieren soll.
	 *
	 *  \todo Konstruktor implementieren
	 */
	Thread(void* tos);

	/*! \brief Verkettungszeiger für Scheduler und Waitingroom */
	QueueLink<Thread> queue_link;
private:
	struct toc regs;
	volatile bool kill_flag;
public:
	/*! \brief Aktiviert den ersten Thread auf einem Prozessor.
	 *
	 *  Durch den Aufruf von Thread::go() wird der erste Thread auf dem
	 *  jeweiligen Prozessor gestartet. Alle weiteren Threadwechsel sollen
	 *  dann mit Hilfe der Methode Thread::resume() ausgeführt werden.
	 *
	 *  \todo Methode implementieren
	 *
	 */
	void go();
	/*! \brief Wechsel von dem aktuell laufenden Thread zum nächsten.
	 *
	 *  Die aktuelle Belegung der nicht-flüchtigen Register wird in dem toc
	 *  Element gesichert und durch die Werte von next (dem toc Element des
	 *  nächsten Threads) ersetzt.
	 *  \param next Zeiger auf den nächsten Thread, der laufen soll.
	 *
	 *  \todo Methode implementieren
	 *
	 */
	void resume(Thread *next);
	/*! \brief Methode, die als Thread ausgeführt werden soll.
	 *
	 *  Abgeleitete Klassen können durch Überschreiben dieser Methode den Code
	 *  angeben, der als Thread ausgeführt werden soll.
	 */
	virtual void action() = 0;

	// von MPStuBS benötigt
	void reset_kill_flag();

	void set_kill_flag();

	bool dying();
};
