/***************************************************
        @author:        bboortz
        @license:       bsd license
***************************************************/




#ifndef NFCLib_h
#define NFCLib_h

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>
#include <Adafruit_NFCShield_I2C.h>


// pins
#define NFC_IRQ_PIN				(2)
#define	NFC_RESET_PIN				(3)	// Not connected by default on the NFC Shield

// keys
#define NFC_KEYLENGTH				(6)
// uint8_t NFC_DEFAULT_KEY2[NFC_KEYLENGTH] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

class NFCLib{
 public:
  NFCLib();
  void setupHost();
  void enableWrite();

  // authentication functions  
  uint8_t nfcAuthSector(uint8_t uidLength, uint8_t* uid, uint8_t sector, uint8_t *key);
  uint8_t nfcCheckAuthBlock(uint8_t block);
  
  // print functions
  uint8_t initTarget(uint8_t *uid);
  uint8_t printTarget();
  uint8_t nfcPrintBlock(uint8_t block);
  uint8_t nfcPrintSector(uint8_t sector);

  // write functions
  uint8_t nfcWriteBlock(uint8_t block, uint8_t *bytes);
  uint8_t nfcWriteKey(uint8_t block, uint8_t *bytes);

 private:
  uint8_t _uidLength;
  uint8_t _uid[NFC_KEYLENGTH];
  uint8_t _writeEnabled;
};

#endif // #ifndef NFCLib_h

