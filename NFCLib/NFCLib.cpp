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

#include "NFCLib.h"





/**************************************************************************/
/*! 
    @brief  Setups the NFC host

*/
/**************************************************************************/
void NFCLib::setupHost()
{
	Serial.begin(115200);
	_writeEnabled = false;

#ifdef NFCLIB_VERBOSE
	Serial.print(NFCLIB_MESSAGE_000); 
#endif // #ifdef NFCLIB_VERBOSE

	_nfc.begin();
	uint32_t versiondata = _nfc.getFirmwareVersion();
	if (! versiondata) {
		Serial.println(NFCLIB_MESSAGE_002);
		while (1);           // halt
	}

	// Got ok data, print it out!
#ifdef NFCLIB_VERBOSE
	Serial.print(NFCLIB_MESSAGE_003); 
	Serial.println((versiondata>>24) & 0xFF, HEX);
	Serial.print(NFCLIB_MESSAGE_004); 
	Serial.print((versiondata>>16) & 0xFF, DEC);
	Serial.print(NFCLIB_MESSAGE_005); 
	Serial.println((versiondata>>8) & 0xFF, DEC);
#endif // #ifdef NFCLIB_VERBOSE

	// Set the max number of retry attempts to read from a card
	// This prevents us from waiting forever for a card, which is
	// the default behaviour of the PN532.
	_nfc.setPassiveActivationRetries(0xFF);

	// configure board to read RFID tags
	_nfc.SAMConfig();

#ifdef NFCLIB_VERBOSE
	Serial.println(NFCLIB_MESSAGE_001); 
	Serial.println();
#endif // #ifdef NFCLIB_VERBOSE
}



/**************************************************************************/
/*! 
    @brief  Enables the write mode

*/
/**************************************************************************/
void NFCLib::enableWrite() {
	_writeEnabled = true;
}

/**************************************************************************/
/*! 
    @brief  initializes the target

    @param  uid  The target uid
*/
/**************************************************************************/
uint8_t NFCLib::initTarget(uint8_t *uid)
{
	uint8_t success;
	_uidLength = 0;

	// Wait for an ISO14443A type cards (Mifare, etc.). When one is found
	// 'uid' will be populated with the UID, and uidLength will indicate
	// if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
	success = _nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, _uid, &_uidLength);
	memcpy(uid, _uid, 6);

	if (!success) {
		Serial.println(NFCLIB_MESSAGE_014);    
		return false;
	}

	return _uidLength;
}



/**************************************************************************/
/*! 
@brief  Prints the target

@param  uid  The target uid
*/
/**************************************************************************/
uint8_t NFCLib::printTarget()
{
	// Display some basic information about the card
	Serial.println(NFCLIB_MESSAGE_006);

#ifdef NFCLIB_DEBUG
	Serial.println("Found a ISO/IEC 14443 NFC card"); 
	if (_uidLength == 4) {
		Serial.println(NFCLIB_MESSAGE_008);
	} 
	else if ( _uidLength == 7) {
		Serial.println(NFCLIB_MESSAGE_009);
	}
#endif // #ifdef NFCLIB_DEBUG

#ifdef NFCLIB_DEBUG
	Serial.print(NFCLIB_MESSAGE_010);
	Serial.print(_uidLength, DEC);
	Serial.println(NFCLIB_MESSAGE_015);
#endif // #ifdef NFCLIB_DEBUG
	Serial.print(NFCLIB_MESSAGE_011);
	_nfc.PrintHexChar(_uid, _uidLength);
	Serial.println(NFCLIB_MESSAGE_007);
	Serial.println();

	return _uidLength;
}



/**************************************************************************/
/*! 
@brief  Authenticates againts a sector

@param  uidLength	The length of the target uid
@param  uid  	The target uid
@param  sector	The sector to authenticate aganst 
@param  key		The key
*/
/**************************************************************************/
uint8_t NFCLib::nfcAuthSector(uint8_t uidLength, uint8_t* uid, uint8_t sector, uint8_t *key) {
	uint8_t success;

	success = _nfc.mifareclassic_AuthenticateBlock(uid, uidLength, sector*4, 1, key);
	if (!success) {
		Serial.print(NFCLIB_MESSAGE_012);
		Serial.println(sector, DEC);
		return false;       
	}

	success = nfcCheckAuthBlock(sector*4);
	if (!success) {
		Serial.print(NFCLIB_MESSAGE_013);
		Serial.println(sector, DEC);
		return false;
	}

	return true;
}



/**************************************************************************/
/*! 
    @brief  Check if we are authenticated againts a block

    @param  block 	The block
*/
/**************************************************************************/
uint8_t NFCLib::nfcCheckAuthBlock(uint8_t block) {
	uint8_t data[16];

	// Try to read the contents of block 4
	return _nfc.mifareclassic_ReadDataBlock(block, data);
}



/**************************************************************************/
/*! 
    @brief  Prints a block

    @param  uid  The target uid
*/
/**************************************************************************/
uint8_t NFCLib::nfcPrintBlock(uint8_t block)
{
	uint8_t success;
	uint8_t data[16];

	// Try to read the contents of block 4
	success = _nfc.mifareclassic_ReadDataBlock(block, data);

	if (!success) {
		Serial.print(NFCLIB_MESSAGE_016);
		Serial.print(block, DEC);
		return false;
	}

	// Data seems to have been read ... spit it out
	Serial.print(NFCLIB_MESSAGE_017);
	Serial.print(block, DEC);  
	Serial.print(NFCLIB_MESSAGE_018);
	_nfc.PrintHexChar(data, 16);

	return true;
}



/**************************************************************************/
/*! 
    @brief  Prints a sector (4 blocks)

    @param  uid  The target uid
*/
/**************************************************************************/
uint8_t NFCLib::nfcPrintSector(uint8_t sector)
{
	Serial.print(NFCLIB_MESSAGE_019);
	Serial.print(sector, DEC);  
	Serial.print(NFCLIB_MESSAGE_020);
	Serial.println();

	int i = 0;
	for (i = 0; i <= 3; i++) {
		nfcPrintBlock(sector*4+i);
	}

	Serial.print(NFCLIB_MESSAGE_021);
	Serial.print(sector, DEC);  
	Serial.print(NFCLIB_MESSAGE_022);
	Serial.println();

	return 0;
}




/**************************************************************************/
/*! 
	@brief  write some data to a block

	@param  uid  The target uid
*/
/**************************************************************************/
uint8_t NFCLib::nfcWriteBlock(uint8_t block, uint8_t * bytes) {
	uint8_t success;
	uint8_t data[16];

	if (!_writeEnabled) {
		Serial.println(NFCLIB_MESSAGE_023);
		return false;
	}

#ifdef NFCLIB_DEBUG
	Serial.print(NFCLIB_MESSAGE_024);
	Serial.println(block, DEC);  
#endif //#ifdef NFCLIB_DEBUG

	// write to the block
	memcpy(data, bytes, sizeof data);
	success = _nfc.mifareclassic_WriteDataBlock (block, data);

	if (!success) {
		Serial.print(NFCLIB_MESSAGE_025);
		Serial.print(block, DEC);
		return false;
	}


	// print the content of the block
#ifdef NFCLIB_DEBUG
	success = nfcPrintBlock(block);
	if (success) {
		Serial.println(NFCLIB_MESSAGE_026);
	}
#endif //#ifdef NFCLIB_DEBUG


	return success;
}


/**************************************************************************/
/*! 
    @brief  write some data to a block

    @param  uid  The target uid
*/
/**************************************************************************/
uint8_t NFCLib::nfcWriteKey(uint8_t block, uint8_t * bytes) {
	uint8_t success;
	uint8_t data[16] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xFF, 0x07, 0x80, 0x69,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	};

	if (!_writeEnabled) {
		Serial.println(NFCLIB_MESSAGE_023);
		return false;
	}

	// prepare bytes
	int i;
	for (i = 10; i <= 15; i++) {
		data[i] = bytes[i-10];
	}

	// write block
	return nfcWriteBlock(block, data);

#ifdef NFCLIB_DEBUG
	Serial.println(NFCLIB_MESSAGE_027);
	_nfc.PrintHexChar(data, 16);
#endif //#ifdef NFCLIB_DEBUG

}
