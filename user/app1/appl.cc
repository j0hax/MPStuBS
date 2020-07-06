// vim: set noet ts=4 sw=4:

#include "appl.h"
#include "thread/scheduler.h"

uint32_t stack1[256];
uint32_t stack2[256];
uint32_t stack3[256];
uint32_t stack4[256];
uint32_t stack5[256];
uint32_t stack6[256];

Application a1(1, stack1+255);
Application a2(2, stack2+255);
Application a3(3, stack3+255);
Application a4(4, stack4+255);
Application a5(5, stack5+255);
Application a6(6, stack6+255);
Ticketlock ticket;

void Application::action() {
  //DBG << "Called " << __FUNCTION__ << "() in " << __FILE__ << endl;
  // ##### start testing #####
  // ###### end testing ######

  // Endlosschleife

  CGA_Screen::Attribute def_att = kout.get_attribute();

  for (int i = 0; ; i++) {
    
    ticket.lock();

      kout.setpos(10,instanceID*2);
      kout << "<thread " << instanceID << ": running task " << flush;

      kout.set_attribute(c2);
      kout << i << flush;
      kout.set_attribute(def_att);

      kout << " on CPU " << system.getCPUID() << "> " << flush;

    ticket.unlock();
    

    /* --start--
    making some really slow operations (volatile)
    to add some delay between thread switching
    */
    /*volatile long long sum = 0;
    for(unsigned long long j = 0; j < 100000000; j++){
      sum += j;
    }*/
    // --end--

    //scheduler.resume();
    
    

    // testing kill and exit
    /*if(instanceID == 2){
      scheduler.kill(&a1);
    }if(instanceID == 3){
      scheduler.exit();
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
    if(instanceID == 2){
      scheduler.kill(&a1);
    }
  */
  }
}
