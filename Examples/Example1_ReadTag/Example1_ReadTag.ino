/*
	SparkFun Qwiic RFID board fot the ID-xxLA Modules
	By: Elias Santistevan
	Sparkfun Electronics
	Date: February, 2018
	License: This code is public domain but if you use this and we meet someday, get me a beer! 

	Feel like supporting our work? Buy a board from Sparkfun!
	https://www.sparkfun.com/products/15191

	This example requests an RFID tag's ID when the interrupt pin from the
  SparkFun Qwiic RFID board goes LOW indicating a tag has been scanned. 
*/

#include <Wire.h> 

#define RFID_ADDR 0x7D // Default I2C address 
//#define RFID_ADD 0x7C // Close "ADDR" jumper for this address
#define TAG_REQUEST 6

// We'll use a pin attached to the interrupt line to initiate the check for the RFID tag ID. 
// Alternately the product can store up to 20 tags. 
const int eventPin = 8;  

void setup()
{
  // Begin I-squared-C
	Wire.begin(); 
	Serial.begin(9600); 
	Serial.println("SparkFun Qwiic RFID, waiting for RFID to be scanned."); 
	pinMode(eventPin, INPUT_PULLUP);// Our pin is active low so let's put it in a known high state.
}
void loop()
{
  // When the interrupt pin changes to a LOW state, a tag has been scanned. 
	if( digitalRead(eventPin) == LOW ) checkTagID();
	delay(250); // Slow it down
}

// 20 tags can be stored by the product at a time, the first one to be scanned is the first one
// to be pulled from the tag stack. If the tag reads '000000' or the interrupt line never went low
// then there are no new tags waiting to be read. 
void checkTagID()
{	
  byte tempTag = 0; 
  // This variable stores the tag and could easily be a global variable. Just
  // make sure to clear it in between reads. 
  String tagID; 
	Serial.print("RFID Tag ID: "); 
	Wire.requestFrom((uint8_t)RFID_ADDR, TAG_REQUEST); 
	for( int x = 0; x < TAG_REQUEST; x++ ) {
    tempTag = Wire.read(); 
    // Concatenating the bytes onto the end of "tagID".
    tagID += String(tempTag); 
  }
  Serial.println(tagID); 
}
