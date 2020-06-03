// vim: set noet ts=4 sw=4:

#include "appl.h"

void Application::action() {
    //DBG << "Called " << __FUNCTION__ << "() in " << __FILE__ << endl;

    // ##### start testing #####
    {
        Secure s;
    
    }
    // ###### end testing ######

    // Endlosschleife    
    for(int i = 0;;i++) {
        
        CPU::disable_int();
        ticketlock.lock();
        DBG_VERBOSE << i << " locked" << endl;
        
        kout.setpos(5,instanceID + 5);
        kout << i << flush;
        
        
        
        ticketlock.unlock();
    CPU::enable_int();
        //DBG << i << " unlocked" << endl;
        
        // uncomment this for seeing output in CPU_0..3 windows
        //DBG << i << endl;
    }
}
