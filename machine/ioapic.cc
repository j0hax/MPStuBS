// vim: set noet ts=4 sw=4:

#include "ioapic.h"
#include "../debug/output.h"

IOAPIC ioapic;

volatile uint32_t *IOAPIC::IOREGSEL_REG = (volatile uint32_t*)0xfec00000;
volatile uint32_t *IOAPIC::IOWIN_REG = (volatile uint32_t*)0xfec00010;

void IOAPIC::init(){

    DBG << "init ioapic" << endl;

    union IOAPICRegister reg;

    // setup IOAPIC_ID
    *IOREGSEL_REG = 0x0;
    reg.value = *IOWIN_REG;
    reg.IOAPICID.ID = system.getIOAPICID();
    *IOWIN_REG = reg.value;
    //DBG << hex << (reg.value) << endl;

    uint8_t cpumask = 0;
    for(uint8_t i = 0; i < system.getNumberOfCPUs(); i++){
        cpumask = (cpumask << 1) + 1;
    }
    //DBG << bin << (unsigned short)cpumask << flush;

    for(int i = 0; i < 24; i++){
        
        //DBG << "IRT Cell " << hex << i*2 + 0x10 << endl;

        *IOREGSEL_REG = i*2 + 0x10;
        reg.value = *IOWIN_REG;
        reg.IOREDTBL_L.vector = 0;
        reg.IOREDTBL_L.delivery_mode = DELIVERY_MODE_LOWESTPRI;
        reg.IOREDTBL_L.destination_mode = DESTINATION_MODE_LOGICAL;
        reg.IOREDTBL_L.polarity = POLARITY_HIGH;
        reg.IOREDTBL_L.trigger_mode = TRIGGER_MODE_EDGE;
        reg.IOREDTBL_L.mask = MASK_DISABLED;
        *IOWIN_REG = reg.value;

        *IOREGSEL_REG = i*2 + 0x10 + 1;
        reg.value = *IOWIN_REG;
        reg.IOREDTBL_H.logical_destination = cpumask;
        *IOWIN_REG = reg.value;
    }   

    // config keyboard for debugging
    /*for (int i = 0; i < 24; i++){
         config(i, Plugbox::Vector::keyboard);
    }*/
}

void IOAPIC::config(unsigned char slot, Plugbox::Vector vector){

    union IOAPICRegister reg;

    *IOREGSEL_REG = slot*2 + 0x10;
    reg.value = *IOWIN_REG;
    //DBG << bin << reg.value << endl;
    reg.IOREDTBL_L.vector = vector;

    reg.IOREDTBL_L.trigger_mode = TRIGGER_MODE_EDGE;

    //reg.IOREDTBL_L.mask = MASK_ENABLED;
    *IOWIN_REG = reg.value;
    //DBG << bin << reg.value << endl;

}

void IOAPIC::allow(unsigned char slot){

    union IOAPICRegister reg;

    *IOREGSEL_REG = slot*2 + 0x10;
    reg.value = *IOWIN_REG;
    reg.IOREDTBL_L.mask = MASK_ENABLED;
    *IOWIN_REG = reg.value;

}

void IOAPIC::forbid(unsigned char slot){

    union IOAPICRegister reg;

    *IOREGSEL_REG = slot*2 + 0x10;
    reg.value = *IOWIN_REG;
    reg.IOREDTBL_L.mask = MASK_DISABLED;
    *IOWIN_REG = reg.value;

}

bool IOAPIC::status(unsigned char slot){

    union IOAPICRegister reg;

    *IOREGSEL_REG = slot*2 + 0x10;
    reg.value = *IOWIN_REG;
    return reg.IOREDTBL_L.mask? false : true;

}
