// vim: set noet ts=4 sw=4:

#include "device/watch.h"

bool Watch::windup(uint32_t us){

    uint64_t ticks = us*lapic.timer_ticks();
    ticks = Math::div64(ticks, 1000);
    
    DBG << lapic.timer_ticks() << endl;
    DBG << "t:" << (unsigned long)ticks << endl;


    _divide = 1;

    while(ticks > (uint64_t)(1 << 31)) {

        ticks = ticks >> 1;
        
        _divide = _divide * 2;


        DBG << (unsigned long)ticks << endl;


        if (_divide > (128)) return false;
    }

    _intv = (uint32_t)ticks;

    DBG << "intv:" << (unsigned int)_intv
        << " div:" << (unsigned int)_divide << endl;

    lapic.setTimer(0, _divide, Plugbox::Vector::timer, true , false);
    plugbox.assign(Plugbox::Vector::timer, this);

    return true;

}

bool Watch::prologue() {

    return true;

}

void Watch::epilogue() {

    DBG << "context change epilogue" << endl;

}

uint32_t Watch::interval() {

    return _intv*_divide/lapic.timer_ticks();

}

void Watch::activate() {

    lapic.setTimer(_intv, _divide, Plugbox::Vector::timer, true , false);

}
