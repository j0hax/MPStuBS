// vim: set noet ts=4 sw=4:

#include "o_stream.h"
//#include <stdio.h>

O_Stream &O_Stream::operator << (char c) {
  put(c);
  return *this;
}

O_Stream &O_Stream::operator << (unsigned char c) {
  put((unsigned char) c);
  return *this;
}


O_Stream &O_Stream::operator << (const char* string) {
  for (const char* c = string; *c != '\0'; c++) {
    *this << *c;
  }

  return *this;
}

O_Stream &O_Stream::operator << (bool b) {
  if (b == true) {
    *this << "true";
  } else {
    *this << "false";
  }

  return *this;
}
/*! \brief Darstellung ganzer Zahlen im Zahlensystem zur Basis base
    *
    *  \todo Operator implementieren
    *
    *  \param ival Darzustellende Zahl
    *  \return Referenz auf ein O_Stream Objekt, um Operatoren konkatenieren zu können.
    */
O_Stream &O_Stream::operator << (short ival) {
  *this << (long)ival;
  return *this;
}
/// \copydoc O_Stream::operator<<(short)
O_Stream &O_Stream::operator << (unsigned short ival) {
  *this << (unsigned long)ival;
  return *this;
}
/// \copydoc O_Stream::operator<<(short)
O_Stream &O_Stream::operator << (int ival) {
  *this << (long)ival;
  return *this;
}
/// \copydoc O_Stream::operator<<(short)
O_Stream &O_Stream::operator << (unsigned int ival) {
  *this << (unsigned long)ival;
  return *this;
}
/// \copydoc O_Stream::operator<<(short)
O_Stream &O_Stream::operator << (long ival) {
  if (base == 10 && ival < 0) {
    *this << '-' << (unsigned long)(ival * (-1)) << dec;
  } else {
    *this << (unsigned long)(ival) << dec;
  }

  return *this;
}
/// \copydoc O_Stream::operator<<(short)
O_Stream &O_Stream::operator << (unsigned long ival) {
  char buff[33] = {0};
  int i = 31;

  if (ival == 0) {
    *this << '0';
    return *this;
  }

  while (ival != 0) {
    char rest = ival % base;
    ival = ival / base;
    buff[i] = (rest > 9) ? 'a' + (rest - 10) : '0' + rest;
    i--;
  }

  *this << (buff + i + 1) << dec;
  return *this;
}
/*! \brief Darstellung eines Zeigers als hexadezimale ganze Zahl
    *
    *  \todo Operator implementieren
    *
    *  \param ptr Darzustellender Pointer
    *  \return Referenz auf ein O_Stream Objekt, um Operatoren konkatenieren zu können.
    */
O_Stream &O_Stream::operator << (const void* ptr) {
  *this << hex << (unsigned long)ptr << dec;
  return *this;
}

O_Stream &O_Stream::operator << (O_Stream & (*f)(O_Stream &)) {
  return f(*this);
};

O_Stream &flush(O_Stream &os) {
  os.flush();
  return os;
}

O_Stream &endl(O_Stream &os) {
  os << '\n' << flush;
  return os;
}

/*! \brief Wählt das binäre Zahlensystem aus.
*
*  \todo Modifikator implementieren
*
*  \param os Zu modifizierender Stream
*  \return Referenz auf ein O_Stream Objekt, um Operatoren konkatenieren zu können.
*/
O_Stream &bin(O_Stream &os) {
  os << "0b";
  os.base = 2;
  return os;
}

O_Stream &oct(O_Stream &os) {
  os << '0';
  os.base = 8;
  return os;
}

O_Stream &dec(O_Stream &os) {
  os.base = 10;
  return os;
}

O_Stream &hex(O_Stream &os) {
  os << "0x";
  os.base = 16;
  return os;
}

