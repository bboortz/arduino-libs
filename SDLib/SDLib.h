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




/**** sd chip settings ****/

#define SD_CS 4


// change this to match your SD shield or module;
//     Arduino Ethernet shield: pin 4
//     Adafruit SD shields and modules: pin 10
//     Sparkfun SD shield: pin 8
#define SDLIB_CHIP 4






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
		void writeFile(File f);


};

#endif // #ifndef SDLib_h
