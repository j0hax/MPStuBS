// vim: set noet ts=4 sw=4:

/*! \file
 *  \brief Enthält die Klasse Guarded_Scheduler
 */

#pragma once

#include "thread/thread.h"
#include "guard/secure.h"
#include "thread/scheduler.h"

/*! \brief Systemaufrufschnittstelle zum Scheduler
 *
 *  Der Guarded_Scheduler implementiert die Systemaufrufschnittstelle zum
 *  Scheduler. Die von ihm angebotenen Methoden werden direkt auf die Methoden
 *  des globalen Scheduler-Objekts abgebildet, nur dass ihre Ausführung jeweils
 *  mit Hilfe eines Objekts der Klasse Secure geschützt wird.
 */
class Guarded_Scheduler
{
public:

    static void exit() {
        { 
            Secure s;
            scheduler.exit();
        }
    }

    static void kill(Thread *that) {
        { 
            Secure s;
            scheduler.kill(that);
        }
    }

    static void ready(Thread *that) {
        { 
            Secure s;
            scheduler.ready(that);
        }
    }

    static void resume() {
        { 
            Secure s;
            scheduler.resume();
        }
    }

};
