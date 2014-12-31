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
#include <ScreenLib.h>




/**** default functions ****/

void ScreenLib::setup() {
	Serial.begin(115200);
	Serial.println("---------- setup screen -------->>");
		

	_tft.begin();

	if (!_ts.begin()) {
		Serial.println("Couldn't start touchscreen controller");
		while (1);
	}
	Serial.println("Screen started");
	Serial.print("\theight: ");
	Serial.println( _tft.height() );
	Serial.print("\twidth: ");
	Serial.println( _tft.width() );
	Serial.println("<<-------- setup screen ----------");
	Serial.println();

}




/**** basic functions ****/

void ScreenLib::clearScreen() {
	_tft.fillScreen(SCREENLIB_BLACK);
	clearTopArea();
        clearButtonArea();
        clearMainArea();
        clearBottomArea();
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

void ScreenLib::println(char* str) {
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
                        Serial.print(x);
                        Serial.print(" / "); Serial.print(y);
                        Serial.print(" = "); Serial.print(c);
                        Serial.println();
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
	char c = 0;
  

	// convert x/y coordinates from the touchscreen to x/y coordinates for the array
	xArr = x / BOXSIZE;
	yArr = ((y - BOXSIZE * KEYBOARD_Y_ADD) / BOXSIZE);

	// check keyboard area
	if (xArr < 0 || yArr < 0 || xArr > KEYBOARD_MAX_X-1 || yArr > KEYBOARD_MAX_Y-1) {
		return c;
	}

	// another keyboard area check
	c = (char)_keyboard[yArr][xArr];
	if (c <= 0) {
		return c;
	}


	Serial.print(x); Serial.print(" / "); Serial.print(y);
	Serial.print("\t=> "); 
	Serial.print(xArr); Serial.print(" / "); Serial.print(yArr);
	Serial.print("\t=> "); 
	Serial.print(c);
	Serial.print("\t=> "); 
	Serial.print(millis());
	Serial.println("");

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






/**** draw button functions ****/

/*!
	@brief	draws a button
*/
void ScreenLib::drawButton(uint16_t x, uint16_t y) {
        Serial.print(x); Serial.print(" / "); Serial.print(y);
        Serial.println();
        _tft.fillCircle(x, y, SCREENLIB_BUTTON_SIZE, SCREENLIB_BLUE); 
        Serial.print(_tft.width()); Serial.print(" / "); Serial.print(_tft.height());
        Serial.println();
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

void ScreenLib::writeText(uint16_t y, uint8_t size, char* c) {
	setCursor(SCREEN_TEXT_PADDING, y);
	setTextColor(ILI9341_WHITE);  
	setTextSize(size);
	println(c);
}

void ScreenLib::writeTextToTop(char* c) {
	writeText(SCREEN_TOP_MINY + SCREEN_TEXT_PADDING, 2, c);
}

void ScreenLib::writeTextToBottom(char* c) {
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


/**** getter for dependencies ****/

Adafruit_ILI9341 ScreenLib::getTft() {
	return _tft;
}

Adafruit_STMPE610 ScreenLib::getTouchScreen() {
	return _ts;
}
/*
WidgetLib ScreenLib::getWidget() {
	return _widget;
}
*/

