// vim: set noet ts=4 sw=4:

#include "appl.h"
#include "utils/spinlock.h"

void Application::action() {
    kout << "Called " << __FUNCTION__ << "() in " << __FILE__ << endl;

    // Spinlock instanzieren
    Spinlock l;

    // Endlosschleife
    for(int i = 0;;i++) {
        l.lock();
        dout_CPU0 << i << endl;
        l.unlock();
    }
}
