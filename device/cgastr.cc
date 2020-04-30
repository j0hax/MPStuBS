// vim: set noet ts=4 sw=4:

#include "device/cgastr.h"

void CGA_Stream::flush(){

    print (buffer, pos);
    pos = 0;

}