// vim: set noet ts=4 sw=4:

#include "appl.h"
#include "syscall/guarded_scheduler.h"

uint32_t stack1[256];
uint32_t stack2[256];
uint32_t stack3[256];
uint32_t stack4[256];
uint32_t stack5[256];
uint32_t stack6[256];

Application a1(0, stack1+255);
Application a2(1, stack2+255);
Application a3(2, stack3+255);
Application a4(3, stack4+255);
Application a5(4, stack5+255);
Application a6(5, stack6+255);
//Ticketlock ticket;

void Application::action() {
  //DBG << "Called " << __FUNCTION__ << "() in " << __FILE__ << endl;
  // ##### start testing #####
  // ###### end testing ######

  // Endlosschleife

  for (int i = 0; ; i++) {

    { Secure s;

      kout.setpos(0,instanceID*2+1);
      kout << "<thread " << instanceID << ": running task ";
      kout << i;
      kout << " on CPU " << system.getCPUID() << "> " << flush;

    }
    
    if( i == 1000 && instanceID == 1) Guarded_Scheduler::kill(&a3);
    if( i == 1000 && instanceID == 2) Guarded_Scheduler::kill(&a2);
    /* --start--
    making some really slow operations (volatile)
    to add some delay between thread switching
    */
    /*volatile long long sum = 0;
    for(unsigned long long j = 0; j < 10; j++){
      sum += j;
    }*/
    // --end--

    //Guarded_Scheduler::resume();

    // testing kill and exit
    /*if(instanceID == 2){
      Guarded_Scheduler::kill(&a1);
    }if(instanceID == 3){
      Guarded_Scheduler::exit();
    }*/

    /* old way of switching threads
    if(instanceID == 1){
      dispatcher.dispatch(&a2);
    }else if(instanceID == 2){
      dispatcher.dispatch(&a3);
    }else if(instanceID == 3){
      dispatcher.dispatch(&a4);
    }else if(instanceID == 4){
      dispatcher.dispatch(&a5);
    }else if(instanceID == 5){
      dispatcher.dispatch(&a1);
    }*/

    /*
    if(instanceID == 2 && i == 5){
      Guarded_Scheduler::kill(&a3);
    }*/
  
  }
}
