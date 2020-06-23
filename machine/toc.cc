// vim: set noet ts=4 sw=4:

#include "device/panic.h"
#include "machine/toc.h"
#include "types.h"

void enterpanic(){
    panic.trigger();
}

// TOC_SETTLE: bereitet den Kontext der Koroutine fuer den ersten Aufruf vor.
void toc_settle (struct toc *regs, void *tos, void (*kickoff)(Thread*), Thread *object)
{
    void **stackptr = (void**) tos;
    regs->ebp = (void*)stackptr;

    *stackptr = (void*)object;
    *(stackptr-1) = (void*)&enterpanic;
    *(stackptr-2) = (void*)kickoff;
    
    regs->esp = stackptr-2;

}

