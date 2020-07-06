// vim: set noet ts=4 sw=4:

#include "device/cgastr.h"

CGA_Screen::Attribute k0(CGA_Screen::color::LIGHT_GREY,
                         CGA_Screen::color::BLACK, false);
CGA_Screen::Attribute c0(CGA_Screen::color::BLACK, CGA_Screen::color::BROWN,
                         false);
CGA_Screen::Attribute c1(CGA_Screen::color::LIGHT_GREY, CGA_Screen::color::BLUE,
                         false);

                         
CGA_Screen::Attribute c2(CGA_Screen::color::BLACK, CGA_Screen::color::CYAN, false);
CGA_Screen::Attribute c3(CGA_Screen::color::BLACK, CGA_Screen::color::BROWN, false);

CGA_Stream kout(0, 79, 0, 10, true, k0);
CGA_Stream dout_CPU0(0, 38, 11, 20, false, c0);
CGA_Stream dout_CPU1(39, 79, 11, 20, false, c1);
CGA_Stream dout_CPU2(0, 38, 21, 24, false, c1);
CGA_Stream dout_CPU3(39, 79, 21, 24, false, c0);


void CGA_Stream::flush() {
  // prints a string (buffer) with length (pos) and color (attr)
  print(buffer, pos, attr);
  pos = 0;
}

CGA_Screen::Attribute CGA_Stream::get_attribute() {
  return attr;
}

void CGA_Stream::set_attribute(CGA_Screen::Attribute att) {
  this->attr = att;
}
