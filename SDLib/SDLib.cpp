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

//#include <Adafruit_ILI9341.h>          // the adafruit tft lib
#include <SDLib.h>




/**** default functions ****/

void SDLib::setup() {

	Serial.begin(115200);
	Serial.println("---------- setup sd card controller -------->>");

	// On the Ethernet Shield, CS is pin 4. It's set as an output by default.
	// Note that even if it's not used as the CS pin, the hardware SS pin 
	// (10 on most Arduino boards, 53 on the Mega) must be left as an output 
	// or the SD library functions will not work. 
	pinMode(10, OUTPUT);
//	pinMode(SS, OUTPUT);

	if ( !SD.begin(SDLIB_CHIP) ) {
		Serial.println("Couldn't start sd card controller");
		while (1);
	}
	Serial.println("SD card controller started");
	Serial.println("<<-------- setup screen ----------");
	Serial.println();

}




/**** basic functions ****/

void SDLib::showFile(const char* filepath) {

	// open the file for reading:
	File f = SD.open(filepath);
	if (!f) {
		Serial.print("Cannot open file: ");
		Serial.println(f.name());
	}

	// read from the file until there's nothing else in it:
	while (f.available()) {
		Serial.write(f.read());
	}

	// close the file:
	f.close();
}

void SDLib::writeFile(File f) {
	Serial.println("SDLib::writeFile(..) is curently not implemented!!");
}

