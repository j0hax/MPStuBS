// vim: set noet ts=4 sw=4:

/*! \file
 *  \brief Enthält die Klasse CGA_Stream
 */

/*! \defgroup io I/O Subsystem
	\brief Das Eingabe-/Ausgabe Subsystem

*/

#pragma once

#include "object/o_stream.h"
#include "machine/cgascr.h"

/*! \brief Darstellung verschiedener Datentypen auf dem Bildschirm
 *  \ingroup io
 *
 *
 *  Die Klasse CGA_Stream ermöglicht die Ausgabe verschiedener Datentypen als
 *  Zeichenketten auf dem CGA Bildschirm eines PCs. Dazu braucht CGA_Stream nur
 *  von den Klassen O_Stream und CGA_Screen abgeleitet und endlich die Methode
 *  flush() implementiert werden. Für weitergehende Formatierung oder spezielle
 *  Effekte stehen die Methoden der Klasse CGA_Screen zur Verfügung.
 */
class CGA_Stream : public CGA_Screen, public O_Stream
//TODO: Hier muss die Vererbungshierarchie vervollständigt werden.
{
private:
  // Verhindere Kopien und Zuweisungen
  CGA_Stream(const CGA_Stream &)            = delete;
  CGA_Stream &operator=(const CGA_Stream &) = delete;
  CGA_Screen::Attribute attr;
public:
  /// \copydoc CGA_Screen::CGA_Screen(int, int, int, int, bool)
  CGA_Stream(int from_col, int to_col, int from_row, int to_row,
             bool use_cursor = false, Attribute a = Attribute())
    : CGA_Screen(from_col, to_col, from_row, to_row, use_cursor), attr(a)
      //TODO: Hier muss noch Code vervollständigt werden.
  {
    /*(void) from_col;
    (void) to_col;
    (void) from_row;
    (void) to_row;
    (void) use_cursor;*/
    reset(' ', get_attribute());
  }

  /*! \brief Methode zur Ausgabe des Pufferinhalts der Basisklasse Stringbuffer.
   *
   *  Die Methode wird implizit aufgerufen, sobald der Puffer voll ist,
   *  kann aber auch explizit verwendet werden, um eine Ausgabe zu erzwingen.
   *
   *  \todo Methode implementieren
   */
  void flush();

  CGA_Screen::Attribute get_attribute();

  void set_attribute(CGA_Screen::Attribute att);

};

extern CGA_Stream kout;
extern CGA_Stream dout_CPU0;
extern CGA_Stream dout_CPU1;
extern CGA_Stream dout_CPU2;
extern CGA_Stream dout_CPU3;

extern CGA_Screen::Attribute c2;
extern CGA_Screen::Attribute c3;
