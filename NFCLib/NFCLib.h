/***************************************************
        @author:        bboortz
        @license:       bsd license
***************************************************/




#ifndef NFCLib_h
#define NFCLib_h




/**** includes ****/

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

//#include <Wire.h>
#include <Adafruit_NFCShield_I2C.h>




/**** logging and debugging ****/

// #define NFCLIB_DEBUG
// #define NFCLIB_VERBOSE




/**** messages ****/

#define NFCLIB_MESSAGE_000           "---------- setup nfc controller -------->>"
#define NFCLIB_MESSAGE_001           "<<-------- setup nfc controller ----------"
#define NFCLIB_MESSAGE_002           "Didn't find PN53x board"
#define NFCLIB_MESSAGE_003           "Found chip PN5"
#define NFCLIB_MESSAGE_004           "Firmware ver. "
#define NFCLIB_MESSAGE_005           "."
#define NFCLIB_MESSAGE_006           "----- print target --->>"
#define NFCLIB_MESSAGE_007           "<<--- print target -----"
#define NFCLIB_MESSAGE_008           "Seems to be a Mifare Classic card (4 byte UID)"
#define NFCLIB_MESSAGE_009           "Seems to be a Mifare Ultralight tag (7 byte UID)"
#define NFCLIB_MESSAGE_010           " UID Length: "
#define NFCLIB_MESSAGE_011           " UID Value: "
#define NFCLIB_MESSAGE_012           "ERROR: Authentication failed for sector: "
#define NFCLIB_MESSAGE_013           "ERROR: Access restricted for sector: "
#define NFCLIB_MESSAGE_014           "ERROR: Cannot read Target!"
#define NFCLIB_MESSAGE_015           " bytes"
#define NFCLIB_MESSAGE_016           "ERROR: unable to read block: "
#define NFCLIB_MESSAGE_017           "Block "
#define NFCLIB_MESSAGE_018           " - "
#define NFCLIB_MESSAGE_019           "----- print sector "
#define NFCLIB_MESSAGE_020           " --->>"
#define NFCLIB_MESSAGE_021           "<<--- print sector "
#define NFCLIB_MESSAGE_022           " -----"
#define NFCLIB_MESSAGE_023           "ERROR: cannot write! write not enabled!"
#define NFCLIB_MESSAGE_024           "Writing Block "
#define NFCLIB_MESSAGE_025           "ERROR: unable to write block "
#define NFCLIB_MESSAGE_026           "Write done"
#define NFCLIB_MESSAGE_027           "New Key"
#define NFCLIB_MESSAGE_028           "Found a ISO/IEC 14443 NFC card"






/**** hardware pins ****/

#define NFC_IRQ_PIN			2
#define	NFC_RESET_PIN			3	// Not connected by default on the NFC Shield




/**** keys ****/

#define NFC_KEYLENGTH				(6)
// uint8_t NFC_DEFAULT_KEY2[NFC_KEYLENGTH] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };




/**** class ****/

class NFCLib{
	public:
		NFCLib() 
			: _nfc(NFC_IRQ_PIN, NFC_RESET_PIN)
		{
		}
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

		Adafruit_NFCShield_I2C _nfc;
};

#endif // #ifndef NFCLib_h

