// vim: set noet ts=4 sw=4:

#include "appl.h"

void Application::action() {
    //DBG << "Called " << __FUNCTION__ << "() in " << __FILE__ << endl;

    // Endlosschleife    
    for(int i = 0;;i++) {

        
        CPU::disable_int();
        spinlock.lock();
        //DBG << i << " locked" << endl;
        
        kout.setpos(5,8);
        kout << i << flush;
        
        
        CPU::enable_int();
        spinlock.unlock();

        //DBG << i << " unlocked" << endl;
        
        // uncomment this for seeing output in CPU_0..3 windows
        //DBG << i << endl;
    }
}
