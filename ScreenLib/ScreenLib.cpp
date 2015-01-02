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

#include <ScreenLib.h>




/**** default functions ****/

void ScreenLib::setup() {
	Serial.begin(115200);

#ifdef SCREENLIB_VERBOSE
	Serial.println(SCREENLIB_MESSAGE_000);
#endif // #ifdef SCREENLIB_VERBOSE
		
	_tft.begin();

	if (!_ts.begin()) {
		Serial.println(SCREENLIB_MESSAGE_002);
		while (1);
	}

#ifdef SCREENLIB_VERBOSE
	Serial.println(SCREENLIB_MESSAGE_003);
	Serial.print(SCREENLIB_MESSAGE_004);
	Serial.print( _tft.height() );
	Serial.print(SCREENLIB_MESSAGE_005);
	Serial.println( _tft.width() );
	Serial.println(SCREENLIB_MESSAGE_001);
	Serial.println();
#endif // #ifdef SCREENLIB_VERBOSE
}




/**** basic functions ****/

void ScreenLib::reset() {
	// reset variables
	_last_key_press_ms = 0;
	_last_key = 0;

	// black the screen
	clearScreen();

	// reset touchscreen
	uint16_t x, y;
	uint8_t z;  
	while (!_ts.bufferEmpty()) {
		_ts.readData(&x, &y, &z);
	}
	_ts.writeRegister8(STMPE_INT_STA, 0xFF);
}

void ScreenLib::clearScreen() {
	_tft.fillScreen(SCREENLIB_BLACK);
}

void ScreenLib::fillScreen(uint16_t color) {
	_tft.fillScreen(color);
}

void ScreenLib::setCursor(uint16_t x, uint16_t y) {
	_tft.setCursor(x, y);
}

void ScreenLib::setTextColor(uint16_t color){
	_tft.setTextColor(color);
}

void ScreenLib::setTextSize(uint8_t size) {
	_tft.setTextSize(size);
}

void ScreenLib::println(const char* str) {
	_tft.println(str);
}





/**** screen area functions ****/

void ScreenLib::clearTopArea() {
	setTextSize(SCREEN_LINE_SIZE);
	drawLine(0, SCREEN_TOP_MAXY, 254, SCREEN_TOP_MAXY, ILI9341_RED);
}

void ScreenLib::clearButtonArea() {
	setTextSize(SCREEN_LINE_SIZE);
	drawButton(SCREEN_TEXT_PADDING*3, SCREEN_BUTTON_MINY + SCREEN_TEXT_PADDING*2.7);
	drawButton(240 -SCREEN_TEXT_PADDING - SCREEN_BUTTON_SIZE, SCREEN_BUTTON_MINY + SCREEN_TEXT_PADDING*2.7);
}

void ScreenLib::clearMainArea() {
	setTextSize(SCREEN_LINE_SIZE);
	drawLine(0, SCREEN_MAIN_MAXY, 254, SCREEN_MAIN_MAXY, ILI9341_RED);
}

void ScreenLib::clearBottomArea() {
	setTextSize(SCREEN_LINE_SIZE);
	fillRect(0, SCREEN_BOTTOM_MINY, 254, BOXSIZE, ILI9341_BLACK);
}





/**** draw keyboard functions ****/

/*! 
    @brief  draws a keyboard
*/
/**************************************************************************/
void ScreenLib::drawKeyboard() {
        int x = 0;
        int y = 0;
        for ( y = 0; y < KEYBOARD_MAX_Y; y++) {
                for ( x = 0; x < KEYBOARD_MAX_X; x++) {
                        char c = (char) _keyboard[y][x];
                        if ( c != 0 ) {
                                drawKeyboardKey(BOXSIZE*x, BOXSIZE*(y + KEYBOARD_Y_ADD), _keyboard[y][x]);
                        }
                }
        }

}

void ScreenLib::drawKeyboardKey(uint16_t x, uint16_t y, char c) {
        drawKeyboardKeyInternal(x, y, c, SCREENLIB_WHITE);
}

void ScreenLib::drawKeyboardKeyPress(uint16_t x, uint16_t y, char c) {
        drawKeyboardKeyInternal(x, y, c, SCREENLIB_RED);
}

char ScreenLib::actOnKeyboardKeyPress(uint16_t x, uint16_t y) {
	int xArr = 0;
	int yArr = 0;
	char cTmp = 0;
	char c = 0;
  

	// convert x/y coordinates from the touchscreen to x/y coordinates for the array
	xArr = x / BOXSIZE;
	yArr = ((y - BOXSIZE * KEYBOARD_Y_ADD) / BOXSIZE);


	// check keyboard area
	if (xArr < 0 || yArr < 0 || xArr > KEYBOARD_MAX_X-1 || yArr > KEYBOARD_MAX_Y-1) {
		return c;
	}

	// another keyboard area check
	cTmp = _keyboard[yArr][xArr];
	if (cTmp <= 0 || cTmp > 127) {
		return c;
	}
	c = cTmp;

	printKeyboardPress(x, y, xArr, yArr, c);
	drawKeyboardKeyPress(  BOXSIZE*xArr, BOXSIZE*(yArr + KEYBOARD_Y_ADD), c);
  

	return c;
}


void ScreenLib::drawKeyboardKeyInternal(uint16_t x, uint16_t y, char c, uint16_t color) {
        _tft.fillRect(x, y, BOXSIZE, BOXSIZE, SCREENLIB_BLUE);
        _tft.drawRect(x, y, BOXSIZE, BOXSIZE, color);
        _tft.setTextColor(color);  _tft.setTextSize(2);
        _tft.setCursor(x+BOXSIZE/3, y+BOXSIZE/3);
        _tft.print(c);
}

void ScreenLib::printKeyboardPress(uint16_t x, uint16_t y, uint16_t xArr, uint16_t yArr, char c) {
#ifdef SCREENLIB_DEBUG	
	Serial.print("Keyboard press: ");
	Serial.print(x); Serial.print(" / "); Serial.print(y);
	Serial.print("\t=> ");
	Serial.print(xArr); Serial.print(" / "); Serial.print(yArr);
	Serial.print("\t=> ");
	Serial.print((int)c);
	Serial.print("\t=> ");
	Serial.print(c);
	Serial.print("\t=> ");
	Serial.print(millis());
	Serial.println("");
#endif // #ifdef SCREENLIB_DEBUG
}







/**** draw button functions ****/

/*!
	@brief	draws a button
*/
void ScreenLib::drawButton(uint16_t x, uint16_t y) {
#ifdef SCREENLIB_DEBUG	
	Serial.print("draw Button: ");
        Serial.print(x); Serial.print(" / "); Serial.print(y);
        Serial.println();
#endif // #ifdef SCREENLIB_DEBUG

        _tft.fillCircle(x, y, SCREENLIB_BUTTON_SIZE, SCREENLIB_BLUE); 
        _tft.drawCircle(x, y, SCREENLIB_BUTTON_SIZE, SCREENLIB_WHITE);

}




/**** draw line functions ****/

void ScreenLib::drawLine(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2, uint16_t color) {
	_tft.drawLine(x, y, x2, y2, color);
}





/**** draw rect functions ****/
void ScreenLib::fillRect(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2, uint16_t color) {
	_tft.fillRect(x, y, x2, y2, color);
}




/**** write text functions ****/

void ScreenLib::writeText(uint16_t x, uint16_t y, uint8_t size, uint16_t color, const char* c) {
	setCursor(x, y);
	setTextColor(color);  
	setTextSize(size);
	println(c);
}

void ScreenLib::writeText(uint16_t y, uint8_t size, const char* c) {
	writeText(SCREEN_TEXT_PADDING, y, size, SCREENLIB_WHITE, c);
}

void ScreenLib::writeTextToTop(const char* c) {
	writeText(SCREEN_TOP_MINY + SCREEN_TEXT_PADDING, 2, c);
}

void ScreenLib::writeTextToBottom(const char* c) {
	clearBottomArea();
	writeText(SCREEN_BOTTOM_MINY + SCREEN_TEXT_PADDING, 1, c);
}


TS_Point ScreenLib::touchscreenGetPoint() {
	uint16_t millis_now = millis();
	uint16_t millis_delta = millis_now - _last_key_press_ms;
	TS_Point p;
	p.x = p.y = p.z = 0;

	// check polling 
	// TODO: We should implement the keyboard polling here. currently it is done by the main sketch

	// TODO: it is only a workaround. A delay is not the best way
/*
	if (millis_delta >= 0 && millis_delta <= KEYBOARD_POLLING_MS) {
		delay(millis_delta);
	}
*/
/*
	if (millis_delta <= KEYBOARD_POLLING_MS) {
		return p;
	}
*/

	// wait for the touch and show it
	if (_ts.bufferEmpty()) {
		return p;
	}
	if (!_ts.touched()) {
		return p;
	}

	// retrieve point
	do {
		p = _ts.getPoint();
	} while (p.z <= MINPRESSURE || p.z >= MAXPRESSURE);

	// Scale from ~0->4000 to screen.getTft().width using the calibration #'s
	p.x = map(p.x, TS_MINX, TS_MAXX, 0, _tft.width());
	p.y = map(p.y, TS_MINY, TS_MAXY, 0, _tft.height());

	// set last key press for polling
	_last_key_press_ms = millis_now;

	return p;
}



