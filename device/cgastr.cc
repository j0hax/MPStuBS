// vim: set noet ts=4 sw=4:

#include "device/cgastr.h"

void CGA_Stream::flush() {
  print(buffer, pos, attr);
  pos = 0;
}

CGA_Screen::Attribute CGA_Stream::get_attribute() {
  return attr;
}
