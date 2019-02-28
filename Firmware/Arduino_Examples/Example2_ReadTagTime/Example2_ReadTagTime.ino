/*
	SparkFun Qwiic RFID board fot the ID-xxLA Modules
	By: Elias Santistevan
	Sparkfun Electronics
	Date: February, 2018
	License: This code is public domain but if you use this and we meet someday, get me a beer! 

	Feel like supporting our work? Buy a board from Sparkfun!
	https://www.sparkfun.com/products/15191

	This example requests an RFID tag's ID and time between when an RFID tag was last
  scanned and when it was requested. 
*/

#include <Wire.h> 

#define RFID_ADDR 0x7D // Default I2C address 
//#define RFID_ADD 0x7C // Close "ADDR" jumper for this address

// The following define the number of bytes the RFID tag uses and an integer
// amount of bytes that indicates the time between when an 
// RFID tag was scanned and when it was last requested. The sum total of these
// two requests from the product is 10 bytes together. 
#define TAG_REQUEST 6
#define TIME_REQUEST 4
#define TAG_TIME_REQUEST 10


// We'll use a pin attached to the interrupt line to initiate the 
// check for the RFID tag ID. Feel free to remove it and do a manual check.
const int eventPin = 8; 

void setup()
{
	Wire.begin(); 
	Serial.begin(9600); 
	Serial.println("Waiting for interrupt to fire which indicates a tag has been read!"); 
	pinMode(eventPin, INPUT_PULLUP); 
}
void loop()
{
	if( digitalRead(eventPin) == LOW ) checkTagID();
	delay(250);// Slow it down
}
// 20 tags can be stored by the Qwiic RFID at a time, the first one to be scanned is the first one
// to be pulled from the tag stack. If the tag reads '000000' or the interrupt line never went low
// then there are no new tags to be read.
void checkTagID()
{	
  // Two temporary variables of interest here, that hold the values that we
  // read off of the I-squared-C buffer before we save it. This helps to
  // clarify how the data is being handled. 
  byte tempTag = 0; 
  long tempTime = 0; 
  int shift = 24; 
  long time = 0; 
  // This variable stores the tag and could easily be a global variable. Just
  // make sure to clear it in between reads. 
  String tagID; 
  

	Wire.requestFrom((uint8_t)RFID_ADDR, TAG_TIME_REQUEST); 
	for( int x = 0; x < TAG_REQUEST; x++ ) {
    tempTag = Wire.read();
    tagID += String(tempTag); 
  }
  for ( int x = 0; x < TIME_REQUEST; x++){
    // Time is fed a byte at a time onto the buffer so we need to put it back
    // in order when we read it. 
    tempTime = Wire.read();
    time |= (tempTime << shift); 
    shift = shift - 8; 
  }
	Serial.print("RFID Tag ID: "); 
  Serial.println(tagID); 
  Serial.print(" Time since last scan: "); 
	// Uncomment the following line to get seconds instead of milliseconds:
	//time /= 1000; 
  Serial.println(time); 


}
