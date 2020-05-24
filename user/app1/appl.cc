// vim: set noet ts=4 sw=4:

#include "appl.h"
//#include "utils/spinlock.h"

void Application::action() {
    DBG << "Called " << __FUNCTION__ << "() in " << __FILE__ << endl;

    // Spinlock instanzieren
    //Spinlock l;

    // Endlosschleife
    kout.reset(' ', kout.get_attribute());
    for(int i = 0;;i++) {

        CPU::disable_int();
        ticketlock.lock();
        
        kout.setpos(5,8);
        kout << i << flush;
        
        ticketlock.unlock();
        CPU::enable_int();
        
        // uncomment this for seeing output in CPU_0..3 windows
        //DBG << i << endl;
    }
}
