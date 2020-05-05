// vim: set noet ts=4 sw=4:

#include "machine/cgascr.h"
// Verwendung der Klasse IO_Port für Zugriff auf die Register
#include "machine/io_port.h"

char* CGA_Screen::screen = (char*)0xb8000;
IO_Port i_port(0x3d4);
IO_Port d_port(0x3d5);

CGA_Screen::CGA_Screen(int from_col, int to_col, int from_row, int to_row,
                       bool use_cursor)
  : from_col(from_col), to_col(to_col), from_row(from_row), to_row(to_row),
    use_cursor(use_cursor) {
  width = to_col - from_col + 1;
  height = to_row - from_row + 1;
}

void CGA_Screen::setpos(int x, int y) {
  if (x < 0 || x >= width || y < 0 || y >= height) {
    return;
  }

  int total_x = from_col + x;
  int total_y = from_row + y;

  if (use_cursor) {
    uint16_t data = (uint16_t)(total_x + total_y * 80);
    i_port.outb(0xe);
    d_port.outb((uint8_t)((data & 0xFF00) >> 8));
    i_port.outb(0xf);
    d_port.outb((uint8_t)data & 0x00FF);
  } else {
    rel_x = x;
    rel_y = y;
  }
}

void CGA_Screen::getpos(int &x, int &y) {
  if (use_cursor) {
    int total_x = 0;
    int total_y = 0;
    uint16_t data = 0;
    i_port.outb(0xe);
    data = 0xFF00 & (d_port.inb() << 8);
    i_port.outb(0xf);
    data += 0xFF & d_port.inb();
    total_y = data / 80;
    total_x = data - total_y * 80;
    y = total_y - from_row;
    x = total_x - from_col;
  } else {
    x = rel_x;
    y = rel_y;
  }
}


void CGA_Screen::print(char* string, int length, Attribute attrib) {
  int i = 0;
  int tab = 0;

  while (i < length) {
    if (string[i] == '\0') {
      ++i;
      continue;
    }

    bool new_line = false;
    bool last_line = false;
    bool backspace = false;
    int x = 0;
    int y = 0;
    getpos(x, y);

    if (string[i] == '\b') {
      backspace = true;

      if (x > 0) {
        x--;
      }
    } else if (x == width - 1 || string[i] == '\n') {
      new_line = true;

      if (y == height - 1) {
        last_line = true;
      }
    }

    if (string[i] == '\t') {
      ++tab;

      if (tab < 4) {
        show(from_col + x, from_row + y, ' ', attrib);
      } else {
        tab = 0;
        ++i;
      }
    } else if (backspace) {
      show(from_col + x, from_row + y, ' ', attrib);
      --x;
      ++i;
    } else if (string[i] != '\n') {
      show(from_col + x, from_row + y, string[i], attrib);
      ++i;
    } else {
      ++i;
    }

    if (last_line) {
      for (int tmp_y = from_row; tmp_y < to_row; ++tmp_y) {
        for (int tmp_x = from_col; tmp_x <= to_col; ++tmp_x) {
          screen[(tmp_x + tmp_y * 80) * 2] = screen[(tmp_x + (tmp_y + 1) * 80) * 2];
          screen[(tmp_x + tmp_y * 80) * 2 + 1] = screen[(tmp_x + (tmp_y + 1) * 80) * 2 +
                                                 1];
        }
      }

      //clear last line
      for (int c = 0; c < width; ++c) {
        show(from_col + c, from_row + y, ' ', attrib);
      }

      x = 0;
    } else if (new_line) {
      ++y;
      x = 0;
    } else {
      ++x;
    }

    setpos(x, y);
    /*
    if(string[i] == '\n'){
        ++y;
        x = 0;
        ++i;
    }else{
        show(from_col + x, from_row + y, string[i], attrib);

        x++;
        i++;

        if ((x == width || string[i] == '\n') && y == height-1){

            for(int tmp_y = from_row; tmp_y < to_row; ++tmp_y){
                for(int tmp_x = from_col; tmp_x <= to_col; ++tmp_x){

                    screen[(tmp_x + tmp_y*80)*2] = screen[(tmp_x + (tmp_y+1)*80)*2];
                    screen[(tmp_x + tmp_y*80)*2 + 1] = screen[(tmp_x + (tmp_y+1)*80)*2 + 1];
                }
            }
            //clear last line
            for(int c = 0; c < width; ++c){
                show(from_col + c, from_row + y, ' ');
            }
            x = 0;
            setpos(x,y);
        } else if (x == width){
            ++y;
            x = 0;
        }
    }
    setpos(x, y);
    */
  }
}


void CGA_Screen::reset(char character, Attribute attrib) {
  for (int x = from_col; x <= to_col; ++x) {
    for (int y = from_row; y <= to_row; ++y) {
      show(x, y, character, attrib);
    }
  }

  setpos(0, 0);
  rel_x = 0;
  rel_y = 0;
}

void CGA_Screen::show(int x, int y, char character, Attribute attrib) {
  if (x < 0) {
    x = 80 + x;
  }

  if (y < 0) {
    y = 25 + y;
  }

  if (x < 0 || x > 79 || y < 0 || y > 24) {
    return;
  }

  screen[(x + y * 80) * 2] = character;
  screen[(x + y * 80) * 2 + 1] = *((char*) &attrib);
}
