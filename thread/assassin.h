// vim: set noet ts=4 sw=4:

#include <machine/plugbox.h>
#include <thread/scheduler.h>

/*! \file
 *  \brief Enthält die Klasse Assassin
 */

#pragma once

extern Plugbox plugbox;
extern Scheduler scheduler;

// Ab hier alles selbst geschrieben :/
class Assassin : public Gate {

public:

    // Der Epilog muss nun, falls der aktuell laufende Thread das "Dying-Flag" gesetzt hat, ein Rescheduling auslösen.
    void epilogue();

    // Konfiguriert Plugbox plugbox derart, dass der Assassin beim Auftreten des entsprechenden Interrupts aktiviert wird.
    void hire();

    // Der Prolog muss hier nichts weiter tun als einen Epilog anzufordern.
    bool prologue();

};

extern Assassin assassin;
