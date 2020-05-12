// vim: set noet ts=4 sw=4:

#include "machine/cgascr.h"
// Verwendung der Klasse IO_Port für Zugriff auf die Register


// init screen pointer
CGA_Screen::Cell* CGA_Screen::screen = (CGA_Screen::Cell*)0xb8000;
// init cursor ports

// constructor
CGA_Screen::CGA_Screen(int from_col, int to_col, int from_row, int to_row,
                       bool use_cursor)
  : from_col(from_col), to_col(to_col), from_row(from_row), to_row(to_row),
    use_cursor(use_cursor), i_port(0x3d4), d_port(0x3d5){
  width = to_col - from_col + 1;
  height = to_row - from_row + 1;
}

// set position of virtual or hardware cursor
void CGA_Screen::setpos(int x, int y) {
  // if too big or too small don't to anything
  if (x < 0 || x >= width || y < 0 || y >= height) {
    return;
  }

  // calculate total coordinates in window
  int total_x = from_col + x;
  int total_y = from_row + y;

  // hardware cursor
  if (use_cursor) {
    uint16_t data = (uint16_t)(total_x + total_y * 80);
    // send upper byte
    i_port.outb(0xe);
    d_port.outb((uint8_t)((data & 0xFF00) >> 8));
    // send lower byte
    i_port.outb(0xf);
    d_port.outb((uint8_t)data & 0x00FF);
  } else { // software virtual (invisible) cursor
    rel_x = x;
    rel_y = y;
  }
}

// get position of cursor
void CGA_Screen::getpos(int &x, int &y) {
  if (use_cursor) {
    int total_x = 0;
    int total_y = 0;
    uint16_t data = 0;
    // get upper byte
    i_port.outb(0xe);
    data = 0xFF00 & (d_port.inb() << 8);
    // get lower byte
    i_port.outb(0xf);
    data += 0xFF & d_port.inb();
    // calculate coordinates for data 
    total_y = data / 80;
    total_x = data - total_y * 80;
    y = total_y - from_row;
    x = total_x - from_col;
  } else { // if bno hardware cursor is used
    x = rel_x;
    y = rel_y;
  }
}

// prints a string; used by CGA_Stream as output method
void CGA_Screen::print(char* string, int length, Attribute attrib) {
  
  // index at string
  int i = 0;

  // tab indent 
  int INDENT = 4;
  // remaining spaces to new tab position
  int remain = 0;

  // get x and y relativ to window
  int x = 0;
  int y = 0;
  /* only do this at the beginning of the print
  and update to position at the end of the function */
  getpos(x, y);

  // loop until string is written to console
  while (i < length) {

    // ignore '\0' as a symbol
    if (string[i] == '\0') {
      ++i;
      continue;
    }

    // if input is '\n' or line is full
    bool new_line = false;
    // if current line is last line at window
    bool last_line = false;
    // if input is '\b' = backspace
    bool backspace = false;
    
    
    /*
      checking the next input char
    */
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

    /*
      printing next char
    */
    // tab
    if (string[i] == '\t') {
      if(remain == 0){
        remain = INDENT - (x % INDENT);
      }
      if (remain > 1) {
        --remain;
        show(from_col + x, from_row + y, ' ', attrib);
      } else {
        remain = 0;
        ++i;
      }
    // backspace
    } else if (backspace) {
      show(from_col + x, from_row + y, ' ', attrib);
      --x;
      ++i;
    // new line through '\n'
    } else if (string[i] != '\n') {
      show(from_col + x, from_row + y, string[i], attrib);
      ++i;
    } else {
      ++i;
    }

    /*
      important checks after writing a char    
    */
    // last line is full
    if (last_line) {
      // move all from last line - 1 one line up
      for (int tmp_y = from_row; tmp_y < to_row; ++tmp_y) {
        for (int tmp_x = from_col; tmp_x <= to_col; ++tmp_x) {
          screen[(tmp_x + tmp_y * 80)] = screen[(tmp_x + (tmp_y + 1) * 80)];
          //screen[(tmp_x + tmp_y * 80) * 2 + 1] = screen[(tmp_x + (tmp_y + 1) * 80) * 2 +1];
        }
      }

      //clear last line
      for (int c = 0; c < width; ++c) {
        show(from_col + c, from_row + y, ' ', attrib);
      }

      x = 0;
    } else if (new_line) { // if newline 
      ++y;
      x = 0;
    } else { // else just move cursor by one (most of the time that's the case)
      ++x;
    }
  }
  // set the new cursor position
  setpos(x, y);
}

// clears whole window and resets cursor
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

// responsible for actually writing a character to screen
void CGA_Screen::show(int x, int y, char character, Attribute attrib) {

  // punch everything in a Cell structure
  struct CGA_Screen::Cell cell;
  cell.c = character;
  cell.a = attrib;

  // if negativ come from other side
  if (x < 0) {
    x = 80 + x;
  }
  if (y < 0) {
    y = 25 + y;
  }

  // if too large/small do nothing
  if (x < 0 || x > 79 || y < 0 || y > 24) {
    return;
  }

  // write the structure to memory
  screen[x + y * 80] = cell;
  //screen[(x + y * 80) * 2 + 1] = *((char*) &attrib);
}
