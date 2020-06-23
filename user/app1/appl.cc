// vim: set noet ts=4 sw=4:

#include "appl.h"

uint32_t stack1[512];
uint32_t stack2[512];

Application a1(1, stack1+511);
Application a2(2, stack2+511);

void Application::action() {
  //DBG << "Called " << __FUNCTION__ << "() in " << __FILE__ << endl;
  // ##### start testing #####
  // ###### end testing ######

  // Endlosschleife
  for (int i = 0; ; i++) {
    
    kout.setpos(10,instanceID*2);
    kout << "<thread " << instanceID << ": hello No." << i << "> " << endl;

    /* --start--
    making some really slow operations (volatile)
    to add some delay between thread switching
    */
    volatile long long sum = 0;
    for(unsigned long long j = 0; j < 200000000; j++){
      sum += j;
    }
    // --end--
    
    // switching thread
    if(instanceID == 1){
      resume(&a2);
    }else if(instanceID == 2){
      resume(&a1);
    }
  }
}
