#include "assert.h"

[[noreturn]] void assertion_failed(const char * exp, const char * func, const char * file, int line){
	(void) exp;
	(void) func;
	(void) file;
	(void) line;
	// TODO: Hier sollte eine Fehlermeldung ausgegeben werden.
	// TODO: Anschließend sollte der Prozessor dauerhaft angehalten.werden, die Klasse CPU bietet eine hilfreiche Methode
	while(1);
}

