// vim: set noet ts=4 sw=4:

#include "strbuf.h"

void Stringbuffer::put(char c){
    buffer[pos] = c;
    pos++;
    if(pos == 80){
        flush(); 
    }
}
