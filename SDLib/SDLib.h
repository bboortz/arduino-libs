/***************************************************
        @author:        bboortz
        @license:       bsd license
***************************************************/



#ifndef SDLib_h
#define SDLib_h




/**** includes ****/

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <SPI.h>			// the arduino spi lib
#include <SD.h>				// the arduino sd lib




/**** logging and debugging ****/

// #define SDLIB_DEBUG
// #define SDLIB_VERBOSE




/**** buffer settings ****/

#define SDLIB_STRING_BUF 128




/**** messages ****/

#define SDLIB_MESSAGE_000           "---------- setup sd card controller -------->>"
#define SDLIB_MESSAGE_001           "<<-------- setup sd card controller ----------"
#define SDLIB_MESSAGE_002           "Couldn't start sd card controller"
#define SDLIB_MESSAGE_003           "SD card controller started"
#define SDLIB_MESSAGE_004           "Cannot open file: "
#define SDLIB_MESSAGE_NOTIMPL		"Method is currently not implemented"




/**** sd chip settings ****/

#define SD_CS 4

// change this to match your SD shield or module;
//     Arduino Ethernet shield: pin 4
//     Adafruit SD shields and modules: pin 10
//     Sparkfun SD shield: pin 8
#define SDLIB_CS	4		// CS PIN = CHIP_SELECT PIN






/**** class ****/

class SDLib{
	public:

		// default functions
		SDLib()
		{
		}
		void setup();

		// basic file functions
		void showFile(const char* filepath);
		char* readFile(const char* filepath);
//		void writeFile(File f);

};

#endif // #ifndef SDLib_h
