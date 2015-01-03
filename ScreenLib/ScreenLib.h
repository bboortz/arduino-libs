/***************************************************
        @author:        bboortz
        @license:       bsd license
***************************************************/



#ifndef ScreenLib_h
#define ScreenLib_h

/**** includes ****/

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

//#include <Adafruit_GFX.h>              // the adafruit core graphics lib
#include <Adafruit_ILI9341.h>          // the adafruit tft lib
#include <Adafruit_STMPE610.h>         // the adafruit touch lib




/**** logging and debugging ****/

// #define SCREENLIB_DEBUG
// #define SCREENLIB_VERBOSE




/**** messages ****/

#define SCREENLIB_MESSAGE_000		"---------- setup screen -------->>"
#define SCREENLIB_MESSAGE_001		"<<-------- setup screen ----------"
#define SCREENLIB_MESSAGE_002		"Couldn't start touchscreen controller"
#define SCREENLIB_MESSAGE_003		"Screen started"
#define SCREENLIB_MESSAGE_004		"Screen Resolution: "
#define SCREENLIB_MESSAGE_005		" x "





/**** color settings ****/

#define SCREENLIB_WHITE		ILI9341_WHITE
#define SCREENLIB_BLACK		ILI9341_BLACK
#define SCREENLIB_RED		ILI9341_RED
#define SCREENLIB_BLUE		ILI9341_BLUE




/**** keyboard settings ****/

#define KEYBOARD_MAX_X 10
#define KEYBOARD_MAX_Y 4
#define KEYBOARD_Y_ADD 4

#define KEYBOARD_POLLING_MS 150

#define BOXSIZE 28




/**** button settings ****/

#define SCREENLIB_BUTTON_SIZE   13






/**** screen settings ****/

#define SCREEN_TOP_MINY 0
#define SCREEN_TOP_MAXY BOXSIZE * 1

#define SCREEN_BUTTON_MINY SCREEN_TOP_MAXY + 1
#define SCREEN_BUTTON_MAXY BOXSIZE * 2

#define SCREEN_MAIN_MINY SCREEN_BUTTON_MAXY + 1
#define SCREEN_MAIN_MAXY BOXSIZE * 10

#define SCREEN_BOTTOM_MINY SCREEN_MAIN_MAXY + 1
#define SCREEN_BOTTOM_MAXY BOXSIZE * 11

#define SCREEN_TEXT_PADDING 5
#define SCREEN_BUTTON_SIZE 12
#define SCREEN_LINE_SIZE 3




/**** tft settings ****/

// The display also uses hardware SPI, plus #9 & #10
#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST -1




/**** touchscreen settings ****/

// The STMPE610 uses hardware SPI on the shield, and #8
#define STMPE_CS 8


#define TS_MINX 150
#define TS_MINY 130
#define TS_MAXX 3800
#define TS_MAXY 4000

#define MINPRESSURE 10
#define MAXPRESSURE 1000




/**** class ****/

class ScreenLib{
	public:

		// default functions
		ScreenLib(uint8_t cs_pin, uint8_t dc_pin, uint8_t rst_pin = -1)
			: _tft (cs_pin, dc_pin)

		{
			_ts = Adafruit_STMPE610(STMPE_CS);

			// set keyboard
			_keyboard[0][0] = '1';
			_keyboard[0][1] = '2';
			_keyboard[0][2] = '3';
			_keyboard[0][3] = '4';
			_keyboard[0][4] = '5';
			_keyboard[0][5] = '6';
			_keyboard[0][6] = '7';
			_keyboard[0][7] = '8';
			_keyboard[0][8] = '9';
			_keyboard[0][9] = '0';

			_keyboard[1][0] = 'q';
			_keyboard[1][1] = 'w';
			_keyboard[1][2] = 'e';
			_keyboard[1][3] = 'r';
			_keyboard[1][4] = 't';
			_keyboard[1][5] = 'y';
			_keyboard[1][6] = 'u';
			_keyboard[1][7] = 'i';
			_keyboard[1][8] = 'o';
			_keyboard[1][9] = 'p';

			_keyboard[2][0] = 'a';
			_keyboard[2][1] = 's';
			_keyboard[2][2] = 'd';
			_keyboard[2][3] = 'f';
			_keyboard[2][4] = 'g';
			_keyboard[2][5] = 'h';
			_keyboard[2][6] = 'j';
			_keyboard[2][7] = 'k';
			_keyboard[2][8] = 'l';
			_keyboard[2][9] = 0;

			_keyboard[3][0] = 'z';
			_keyboard[3][1] = 'x';
			_keyboard[3][2] = 'c';
			_keyboard[3][3] = 'v';
			_keyboard[3][4] = 'b';
			_keyboard[3][5] = 'n';
			_keyboard[3][6] = 'm';
			_keyboard[3][7] = 0;
			_keyboard[3][8] = 0;
			_keyboard[3][9] = 0;

			// preset some variables
			_last_key_press_ms = 0;
		        _last_key = 0;
		}
		void setup();

		// basic screen functions
		void reset();
		void clearScreen();
		void drawStandardScreen();
		void fillScreen(uint16_t color);
		void setCursor(uint16_t x, uint16_t y);
		void setTextColor(uint16_t color);
		void setTextSize(uint8_t size);
		void println(const char* str);

		// screen area functions
		void clearTopArea();
		void clearButtonArea();
		void clearMainArea();
		void clearBottomArea();


		// draw keyboard functions
		void drawKeyboard();
		void drawKeyboardKey(uint16_t x, uint16_t y, char c);
		void drawKeyboardKeyPress(uint16_t x, uint16_t y, char c);
		char actOnKeyboardKeyPress(uint16_t x, uint16_t y);

		// draw button functions
		void drawButton(uint16_t x, uint16_t y);
		void drawButtonPress(uint16_t x, uint16_t y);

		// draw line functions
		void drawLine(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2, uint16_t color);

		// draw rect functions
		void fillRect(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2, uint16_t color);

		// write text functions
		void writeText(uint16_t x, uint16_t y, uint8_t size, uint16_t color, const char* c);
		void writeText(uint16_t y, uint8_t size, const char* c);
		void writeTextToTop(const char* c);
		void writeTextToBottom(const char* c);

		// touchscreen functions
		TS_Point touchscreenGetPoint();

	private:
		uint8_t _keyboard[KEYBOARD_MAX_Y][KEYBOARD_MAX_X];
		uint16_t _last_key_press_ms;
		char _last_key;

		Adafruit_ILI9341 _tft;
		Adafruit_STMPE610 _ts;

		void drawKeyboardKeyInternal(uint16_t x, uint16_t y, char c, uint16_t color); 
		void printKeyboardPress(uint16_t x, uint16_t y, uint16_t xArr, uint16_t yArr, char c);
};


#endif // #ifndef ScreenLib_h
