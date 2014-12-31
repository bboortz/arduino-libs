/***************************************************
        @author:        bboortz
        @license:       bsd license
***************************************************/




#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>
#include "Adafruit_NFCShield_I2C.h"

#include "NFCLib.h"

// Uncomment these lines to enable debug output 
// #define NFCLIB_DEBUG


// initialize Afafruit NFCShield
Adafruit_NFCShield_I2C _nfc(NFC_IRQ_PIN, NFC_RESET_PIN);


/**************************************************************************/
/*!
	@brief Instantiates a new nfclib

*/
/**************************************************************************/
NFCLib::NFCLib(void) {
  _writeEnabled = false;
}

/**************************************************************************/
/*! 
    @brief  Setups the NFC host

*/
/**************************************************************************/
void NFCLib::setupHost()
{
  Serial.begin(115200);
  _writeEnabled = false;
  Serial.println("---------- setup NFC host -------->>");

  _nfc.begin();
  uint32_t versiondata = _nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1);           // halt
  }

  // Got ok data, print it out!
  Serial.print("Found chip PN5"); 
  Serial.println((versiondata>>24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); 
  Serial.print((versiondata>>16) & 0xFF, DEC);
  Serial.print('.'); 
  Serial.println((versiondata>>8) & 0xFF, DEC);

  // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  _nfc.setPassiveActivationRetries(0xFF);

  // configure board to read RFID tags
  _nfc.SAMConfig();

  Serial.println("Waiting for an ISO14443A card ...");
  Serial.println("<<-------- setup NFC host ----------");
  Serial.println();
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
    Serial.println("ERROR: Cannot read Target!");    
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
  Serial.println("---------- print target -------->>");
  Serial.println("Found an ISO14443A card");
  if (_uidLength == 4) {
    Serial.println("Seems to be a Mifare Classic card (4 byte UID)");
  } 
  else if ( _uidLength == 7) {
    Serial.println("Seems to be a Mifare Ultralight tag (7 byte UID)");
  }
  Serial.print(" UID Length: ");
  Serial.print(_uidLength, DEC);
  Serial.println(" bytes");
  Serial.print(" UID Value: ");
  _nfc.PrintHexChar(_uid, _uidLength);
  Serial.println("<<-------- print target ----------");
  Serial.println("");

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
  if (!success)
  {
    Serial.print("ERROR: Authentication failed for sector ");
    Serial.print(sector, DEC);
    Serial.println("!");
    return false;       
  }
 

  success = nfcCheckAuthBlock(sector*4);
  if (!success) 
  {
    Serial.println("ERROR: Access restricted");
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

  if (!success)
  {
    Serial.print("ERROR: unable to read block ");
    Serial.print(block, DEC);
    Serial.println("!");
    return false;
  }

  // Data seems to have been read ... spit it out
  Serial.print("Block ");
  Serial.print(block, DEC);  
  Serial.print(" -  ");
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
  Serial.print("---------- print sector ");
  Serial.print(sector, DEC);  
  Serial.print(" -------->>");
  Serial.println();


  int i = 0;
  for (i = 0; i <= 3; i++) {
    nfcPrintBlock(sector*4+i);
  }


  Serial.print("<<-------- print sector ");
  Serial.print(sector, DEC);  
  Serial.print(" ----------");
  Serial.println();
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
    Serial.println("ERROR: write not enabled! Therefore you cannot write data.");
    return false;
  }

  Serial.print("Writing Block ");
  Serial.println(block, DEC);  

  // write to the block
  memcpy(data, bytes, sizeof data);
  success = _nfc.mifareclassic_WriteDataBlock (block, data);

  if (!success) {
    Serial.print("ERROR: unable to write block ");
    Serial.print(block, DEC);
    Serial.println("!");
    return false;
  }


  // print the content of the block
  success = nfcPrintBlock(block);
  if (success) {
    Serial.println("DONE");
  }


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
    Serial.println("ERROR: write not enabled! Therefore you cannot write data.");
    return false;
  }

  // prepare bytes
  int i;
  for (i = 10; i <= 15; i++) {
    data[i] = bytes[i-10];
  }
  Serial.println("NEW KEY: ");
  _nfc.PrintHexChar(data, 16);

  // write block
  return nfcWriteBlock(block, data);
}

