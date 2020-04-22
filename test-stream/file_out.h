// vim: set noet ts=4 sw=4:

/*! \file
 *  \brief Enthält die Klasse FileOut für die C++ Übungsaufgabe
 */

#pragma once

#include "o_stream.h"

/*! \brief Ausgabe in eine Datei
 *
 *  Die Klasse FileOut ermöglicht eine komfortable Ausgabe in eine Datei nur unter
 *  zu Hilfenahme der elementaren Systemaufrufe `open()` / `write()` / `close()` .
 *  Diese Klasse ist von O_Stream abgeleitet.
 */
class FileOut : public O_Stream
{
	// Verhindere Kopien und Zuweisungen
	FileOut(const FileOut&)            = delete;
	FileOut& operator=(const FileOut&) = delete;

    // TODO: evtl fehlen hier noch (private) Attribute
public:
	/*! \brief Konstruktor
	 *  Öffnet die Datei mittels Syscall `open()` zum schreiben.
	 *  \param path Pfad zur Ausgabedatei
	 *
	 *  \todo Konstruktor implementieren
	 */
	FileOut(const char * path);

	/*! \brief Destruktor
	 *  Schließt die Datei (mittels `close()`)
	 *
	 *  \todo Destruktor implementieren
	 */
	virtual ~FileOut();

	/*! \brief Rückgabe des Pfades der Ausgabedatei
	 *  \return Pfad der Ausgabedatei (wie im Konstruktor übergeben)
	 *
	 *  \todo Methode implementieren
	 */
	const char * getPath();

	/*! \brief Abfrage der derzeit (mittels dieser Klasse) geöffneten Dateien
	 *  \return Anzahl der geöffneten Dateien
	 *
	 *  \todo Methode implementieren
	 */
	static int count();

	/*! \brief Schreiben der Zeichenkette in die geöffnete Datei.
	 *  Die Implementierung soll ausschliesslich den Syscall `write()` verwenden.
	 *
	 *  \todo virtuelle Methode implementieren
	 */
	virtual void flush() override;
};

