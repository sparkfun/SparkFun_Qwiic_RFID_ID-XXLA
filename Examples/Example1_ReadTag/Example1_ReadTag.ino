/*
	Qwiic RFID board fot the ID-XXLA Modules
	By: Elias Santistevan
	Sparkfun Electronics
	Date: August 2nd, 2018
	License: This code is public domain but if you use this and we meet someday, get me a beer! 

	Feel like supporting our work? Buy a board from Sparkfun!
	https://www.sparkfun.com/products/14867

	This example will do one thing for you: It will give the RFID tag's ID of the first tag scanned. 
*/

#include <Wire.h> 

#define RFID_ADDR 0x7D //0x7D is the default address, 0x7C if you close the address jumper.

signed long long int tagID; //Length of the tag is 5 bytes long followed by 1 byte of checksum.
signed long long int temp; 

//We'll use a pin attached to the interrupt line to initiate the check for the RFID tag ID. 
//Feel free to remove it and do a manual check.
const int eventPin = 8;  

void setup()
{
	Wire.begin(); 
	Serial.begin(9600); 
	Serial.println("Waiting for interrupt to fire which indicates a tag has been read!"); 
	pinMode(eventPin, INPUT_PULLUP);//Our pin is active low so let's put it in a known high state.
}
void loop()
{
	if( digitalRead(eventPin) == LOW ) checkTagID();
	delay(250);//slow it down
}

//20 tags can be stored by the product at a time, the first one to be scanned is the first one
//to be pulled from the tag stack. If the tag reads '000000' or the interrupt line never went low
//then there are no new tags waiting to be read. 
void checkTagID()
{	
	Serial.print("RFID Tag ID: "); 
	Wire.requestFrom((uint8_t)RFID_ADDR, (uint8_t)6); 
	for( int x = 0; x < 6; x++ ) {
		temp = Wire.read(); 
    tagID |= temp << 32; 
  }
  Serial.println(tagID); 
	Serial.println(); 
}
