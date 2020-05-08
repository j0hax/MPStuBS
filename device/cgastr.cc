// vim: set noet ts=4 sw=4:

#include "device/cgastr.h"

void CGA_Stream::flush() {
  // prints a string (buffer) with length (pos) and color (attr)
  print(buffer, pos, attr);
  pos = 0;
}

CGA_Screen::Attribute CGA_Stream::get_attribute() {
  return attr;
}
