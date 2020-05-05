// vim: set noet ts=4 sw=4:

#include "console_out.h"
#include <stdio.h>

void ConsoleOut::flush() {
  for (int i = 0; i < pos; i++) {
    putchar(buffer[i]);
  }

  pos = 0;
}
