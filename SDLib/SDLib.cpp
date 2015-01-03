/***************************************************
        @author:        bboortz
        @license:       bsd license
***************************************************/



/**** includes ****/

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <SDLib.h>




/**** default functions ****/

void SDLib::setup() {

	Serial.begin(115200);
#ifdef SDLIB_VERBOSE
	Serial.println(SDLIB_MESSAGE_000);
#endif // #ifdef SDLIB_VERBOSE

	// On the Ethernet Shield, CS is pin 4. It's set as an output by default.
	// Note that even if it's not used as the CS pin, the hardware SS pin 
	// (10 on most Arduino boards, 53 on the Mega) must be left as an output 
	// or the SD library functions will not work. 
	pinMode(SS, OUTPUT);

	if ( !SD.begin(SDLIB_CS) ) {
		Serial.println(SDLIB_MESSAGE_002);
		while (1);
	}

#ifdef SDLIB_VERBOSE
	Serial.println(SDLIB_MESSAGE_003);
	Serial.println(SDLIB_MESSAGE_001);
	Serial.println();
#endif // #ifdef SDLIB_VERBOSE

}




/**** basic functions ****/

void SDLib::showFile(const char* filepath) {
	// open the file for reading:
	File f = SD.open(filepath);

	if (!f) {
		Serial.print(SDLIB_MESSAGE_004);
		Serial.println(filepath);
		f.close();
		return;
	}

	// read from the file until there's nothing else in it:
	while (f.available()) {
		Serial.write(f.read());
	}

	// close the file:
	f.close();
}

/*
void SDLib::writeFile(File f) {
	Serial.println(SDLIB_MESSAGE_NOTIMPL);
}
*/

