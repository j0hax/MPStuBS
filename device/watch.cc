// vim: set noet ts=4 sw=4:

#include "device/watch.h"
#include "syscall/guarded_scheduler.h"


Watch watch;

bool Watch::windup(uint32_t us){

    // calculates intv 

    uint64_t tm = lapic.timer_ticks();
    //DBG << "timer:" << (unsigned long)tm << endl;
    uint64_t ticks = us*tm;
    //DBG << "ticks1:" << (unsigned long)ticks << endl;
    ticks = Math::div64(ticks, 1000);
    //DBG << "ticks2:" << (unsigned long)ticks << endl;

    //divide ticks by 2 until ticks fits in 32 bit variable
    while(ticks > (uint32_t)(1 << 31)) {

        ticks = ticks >> 1;
        _divide = _divide << 1;

        DBG << "change: ticks:" << (unsigned long)ticks << " div:" << (int)_divide << endl;

        if (_divide > 128) return false;
    }

    // store ticks in 32 bit _ticks (class attribute)
    _ticks = (uint32_t)ticks;
    
    // talk to plugbox and assign timer interrupt handler
    plugbox.assign(Plugbox::Vector::timer, this);
    // set timer but don't start (setTimer(0,...))
    lapic.setTimer(0, _divide, Plugbox::Vector::timer, true , false);

    return true;

}

bool Watch::prologue() {

    return true;

}

void Watch::epilogue() {
    // test
    DBG << "thread change!" << endl;
    scheduler.resume();
    

}

// returns interval in us
uint32_t Watch::interval() {

    uint64_t us = (uint64_t)_ticks*_divide*1000;
    return (uint32_t)Math::div64(us, lapic.timer_ticks());
}

// actives timer
void Watch::activate() {

    lapic.setTimer(_ticks, _divide, Plugbox::Vector::timer, true , false);

}
