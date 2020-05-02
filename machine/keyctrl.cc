// vim: set noet ts=4 sw=4:

/* INCLUDES */

#include "machine/keyctrl.h"
#include "debug/output.h"

/* GLOBALE VARIABLEN */
//

/* OEFFENTLICHE METHODEN */

// KEYBOARD_CONTROLLER: Initialisierung der Tastatur: alle LEDs werden
//                      ausgeschaltet und die Wiederholungsrate auf
//                      maximale Geschwindigkeit eingestellt.

Keyboard_Controller::Keyboard_Controller () :
	keydecoder(this), ctrl_port (0x64), data_port (0x60)
{
	// alle LEDs ausschalten (bei vielen PCs ist NumLock nach dem Booten an)
	set_led (led_caps_lock, true);
	set_led (led_scroll_lock, true);
	set_led (led_num_lock, false);

	// maximale Geschwindigkeit, minimale Verzoegerung
	set_repeat_rate (0, 0x3);
}

// KEY_HIT: Dient der Tastaturabfrage nach dem Auftreten einer Tastatur-
//          unterbrechung. Wenn der Tastendruck abgeschlossen ist und
//          ein Scancode, sowie gegebenenfalls ein ASCII Code emittelt
//          werden konnte, werden diese in Key zurueckgeliefert. Anderen-
//          falls liefert key_hit () einen ungueltigen Wert zurueck, was
//          mit Key::valid () ueberprueft werden kann.

Key Keyboard_Controller::key_hit()
{
	Key pressed;  // nicht explizit initialisierte Tasten sind ungueltig

	// statusregister checken, wenn Puffer nichts neues hat, returnen
	uint8_t status = ctrl_port.inb();
	
	if ((status & outb) != outb) {
		return pressed;
	}
	
	// Gucken ob daten von Maus -> Dies ist irgendwie nie der Fall??
	if ((status & auxb) == auxb) {
		DBG << "mouse detected" << endl;
		drainKeyboardBuffer();
		return pressed;
	}

	// Key lesen
	pressed = keydecoder.decode(data_port.inb());

	if(pressed.valid())
	DBG << pressed << " detected" << endl;
	
	// Tastatur sagen dass die Key gelesen wurde
	// Key zurueckgeben
	return pressed;
}

// REBOOT: Fuehrt einen Neustart des Rechners durch. Ja, beim PC macht
//         das der Tastaturcontroller.

void Keyboard_Controller::reboot ()
{
	const IO_Port CMOS_ctrl(0x70);
	const IO_Port CMOS_data(0x71);
	const IO_Port system_control_portA(0x92);
	DBG_VERBOSE << "rebooting smp" << endl;
	CMOS_ctrl.outb(0xe + 1);
	CMOS_data.outb(0);
	system_control_portA.outb(0x3);
}
// SET_REPEAT_RATE: Funktion zum Einstellen der Wiederholungsrate der
//                  Tastatur. delay bestimmt, wie lange eine Taste ge-
//                  drueckt werden muss, bevor die Wiederholung einsetzt.
//                  Erlaubt sind Werte zwischen 0 (minimale Wartezeit)
//                  und 3 (maximale Wartezeit). speed bestimmt, wie
//                  schnell die Tastencodes aufeinander folgen soll.
//                  Erlaubt sind Werte zwischen 0 (sehr schnell) und 31
//                  (sehr langsam).

void Keyboard_Controller::set_repeat_rate (int speed, int delay)
{
	/*	Hackerman bitwise concatenation:
		Delay, dann speed hintereinander
	*/
	unsigned char data = (delay << 4) | speed;

	send_command(kbd_cmd::set_speed, data);
}

// SET_LED: setzt oder loescht die angegebene Leuchtdiode

void Keyboard_Controller::set_led (led_t led, bool on)
{
	// Compilen tut das. Funktioniert es auch so? Muss getestet werden.
	//send_command(kbd_cmd::set_led, led);
	// Kommando und LED wurde geschickt, jetzt Zustand
	//send_byte(on);
	if((led & led_t::led_caps_lock) == led_t::led_caps_lock){
		send_command(kbd_cmd::set_led, (uint8_t)(on << 2));
	}else if((led & led_t::led_num_lock) == led_t::led_num_lock){
		send_command(kbd_cmd::set_led, (uint8_t)(on << 1));
	}else if((led & led_t::led_scroll_lock) == led_t::led_scroll_lock){
		send_command(kbd_cmd::set_led, (uint8_t)(on));
	}
}

void Keyboard_Controller::drainKeyboardBuffer()
{
	// Keys lesen bis es keine mehr gibt?
	uint8_t i = ctrl_port.inb();
	while ((i & outb) == outb || (i & auxb) == auxb) {
		//key_hit();
		data_port.inb();
		i = ctrl_port.inb();
		DBG << "clearing " << flush;
	}

}

void Keyboard_Controller::send_command(unsigned char cmd, unsigned char data)
{
	// Befehlscode 
	send_byte(cmd);
	//while((ctrl_port.inb() & kbd_reply::ack) != kbd_reply::ack){
	//	DBG << "wfack " << flush;
	//}
	// Befehlsdaten
	send_byte(data);
}

void Keyboard_Controller::send_byte(unsigned char byte)
{

	while((ctrl_port.inb() & inpb) == inpb)
		DBG << "wfk " << flush;
	data_port.outb(byte);
	DBG << "s " << flush;

}
