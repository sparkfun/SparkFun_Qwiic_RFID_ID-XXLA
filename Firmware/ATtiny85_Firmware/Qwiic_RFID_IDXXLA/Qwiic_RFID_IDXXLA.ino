/*
  An I2C based KeyPad
  By: Elias Santistevan
  SparkFun Electronics
  Date: 2018
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
	<Product Page Here>

  To install support for ATtiny84 in Arduino IDE: https://github.com/SpenceKonde/ATTinyCore/blob/master/Installation.md
  This core is installed from the Board Manager menu
  This core has built in support for I2C S/M and serial
  If you have Dave Mellis' ATtiny installed you may need to remove it from \Users\xx\AppData\Local\Arduino15\Packages

  To support 400kHz I2C communication reliably ATtiny84 needs to run at 8MHz. This requires user to
  click on 'Burn Bootloader' before code is loaded.
*/

#include <Wire.h>

#include <EEPROM.h>

#include <SoftwareSerial.h>

#include <avr/sleep.h>
#include <avr/power.h> 

#define LOCATION_I2C_ADDRESS 0x01 //Location in EEPROM where the I2C address is stored
#define I2C_ADDRESS_DEFAULT 0x13 
#define I2C_ADDRESS_NO_JUMPER 0x14 

#define COMMAND_CHANGE_ADDRESS 0xC7

volatile byte setting_i2c_address = I2C_ADDRESS_DEFAULT; 

volatile byte responseSize = 10; 
//6 byte ID, followed by 4 bytes of time used to pass data back to master
byte responseBuffer[10]; 

//This struct keeps a record of any tag 'events' i.e. reads. 
//This includes the 10 byte Ascii tag ID, 2 byte Checksum, and the 4 other bytes listed below.
#define TAG_ID_SIZE 16 
#define MAX_TAG_STORAGE 20 //How many tags will be held at a time before the first is overwritten.
struct {
  byte tagID[6]; //The Uniqe identifier of the RFID tag, converted from ASCII to HEX will be six bytes in size.
  unsigned long tagTime; //When was the RFID tag sensed?
} tagEvent[MAX_TAG_STORAGE];

volatile byte newestTag = 0;
volatile byte oldestTag = 0;

const byte addrPin = 0; 
const byte interruptPin = 3; //Pin goes low when a tag event is available
const byte resetPin = 11; 

const byte rxPin = 2; 
const byte txPin = 1; //TX Pin is unconnected, unused, and unnecessary except for Software Serial declaration.

SoftwareSerial atSerial = SoftwareSerial(rxPin, txPin); 

const byte startByte = 0x2; //Beginning Transmission Hex value 
const byte endByte = 0x3; //Ending Transmission Hex value
const byte cRHex = 0xD; //Carriage Return Hex
const byte lFHex = 0xA; //Line Follower Hex 

byte tempInc[TAG_ID_SIZE]; 
byte tempTagID[6];

void setup(void)
{
  pinMode(addrPin, INPUT_PULLUP); //Default HIGH = 0x13
  pinMode(interruptPin, OUTPUT); //Tied high and goes low when an RFID has been scanned.

  pinMode(rxPin, INPUT); 
  pinMode(txPin, OUTPUT); 
	atSerial.begin(9600); //Baud requirement for the ID Module

  //Disable ADC
  ADCSRA = 0;
  //Disble Brown-Out Detect
  MCUCR = bit (BODS) | bit (BODSE);
  MCUCR = bit (BODS);

  //Power down various bits of hardware to lower power usage
  //set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  set_sleep_mode(SLEEP_MODE_IDLE);
  sleep_enable();

  readSystemSettings(); //Load all system settings from EEPROM

  startI2C(); //Determine the I2C address we should be using and begin listening on I2C bus
}

void loop(void)
{
  //Waiting for new RFID cards/capsules to be scanned. 
  if( atSerial.available() )
	{
		while( atSerial.available() ) 
		{	
			if( atSerial.read() != startByte ) break; 
			delay(11);//Necessary to not overwhelm the ID-XXLA module baud rate.
			if( getTagID() ) tagEvent[newestTag].tagTime = millis(); //Get the Tag ID, and verify its' contents, then grab time. 
		}
		if( newestTag++ == MAX_TAG_STORAGE ) newestTag = 0; 
	}

  //Set interrupt pin as needed
  if(newestTag != oldestTag	)
    digitalWrite(interruptPin, LOW); //We have tags on the stack!
  else
    digitalWrite(interruptPin, HIGH); //No tag events to report

  sleep_mode(); //Stop everything and go to sleep. Wake up if I2C event occurs.
}

// We want to convert the tag from it's Ascii representation of a HEX value from the module 
// (12 bytes) to the actual HEX value (6 bytes). The Ascii value is saved into a temporary array 
// and the individual elements of the temporary array are passed to another function where they 
// are converted to the decimal equivalent values and saved into another temporaray array. The value
// is validated and then passed into the final array ready to be sent on the I2C bus.
bool getTagID() 
{
	int j = 0; 
	for( int i = 0; i < TAG_ID_SIZE - 1 ; i ++ ) //TAG_ID_SIZE IS 16, but an index is numerically one less (0-15).
	{
		tempInc[i] = atSerial.read(); 	
		delay(11); //The delay keeps the read commands from outpacing the baud rate of 9600.
	}
	for( int i = 0; i < TAG_ID_SIZE - 1; i ++ ) 
	{
		if( i % 2 == 0 && i !=0 ) j++; 
		if( tempInc[i] == endByte ) break;
		else if( i > 10 && (tempInc[i] == cRHex || tempInc[i] == lFHex)) continue; //Not part of the TAG ID
		else if( i % 2 == 0 ) tempTagID[j] = convertAscii(tempInc[i]) << 4; //MSB
		else ( tempTagID[j] |= convertAscii(tempInc[i]) );//LSB
	}
	j=0;

	if( checkSum(tempTagID) )
	{
		for( int i = 0; i < 6; i++ )
		{
			tagEvent[newestTag].tagID[i] = tempTagID[i];
		}
		return true; 
	} 
	else return false; 
}

// Changes Ascii Values to their decimal representation by subtracting a fixed
// number, best understood by taking a look at an Ascii table. For example: we're 
// changing an Ascii zero, to an actual zero: ASCII "0" = 48 decimal.
byte convertAscii(byte asciiVal)
{
	if( asciiVal >= '0' && asciiVal <= '9' ) return asciiVal -= 48; 
	else if( asciiVal >= 'A' && asciiVal <= 'F' ) return asciiVal -= 55; 
}

//Double checking checksum value given by the tag module just in case something went wrong along the way.
bool checkSum(byte* uncheckedTag)
{
	byte exOr = uncheckedTag[0]; 
	for( int i = 1; i < 5 ; i ++)
	{
		exOr ^= uncheckedTag[i];
	}
	if( exOr == uncheckedTag[5] ) return true; 
	else return false;
}


void receiveEvent(int numberOfBytesReceived)
{
  while (Wire.available())
  {
    //Record bytes to local array
    byte incoming = Wire.read();

    if (incoming == COMMAND_CHANGE_ADDRESS) //Set new I2C address
    {
      if (Wire.available())
      {
        setting_i2c_address = Wire.read();

        //Error check
        if (setting_i2c_address < 0x08 || setting_i2c_address > 0x77)
          continue; //Command failed. This address is out of bounds.

        EEPROM.write(LOCATION_I2C_ADDRESS, setting_i2c_address);

        //Our I2C address may have changed because of user's command
        startI2C(); //Determine the I2C address we should be using and begin listening on I2C bus
      }
    }
  }
}

//Send back a number of bytes via an array, max is 8 bytes at a time, 6 byte TAG ID and 2 byte time stamp.
//When the RFID module gets a request for data from the user, this function is called as an interrupt.
void requestEvent()
{
  loadNextTagToArray(); //Send response buffer
  for (byte x = 0 ; x < responseSize ; x++)
    Wire.write(responseBuffer[x]);
}

//Take the FIFO button press off the stack and load it into the transmit array
void loadNextTagToArray()
{
  if (oldestTag != newestTag)
  {
		for( int i = 0; i < 6; i ++ )
		{
			responseBuffer[i] = tagEvent[oldestTag].tagID[i];
		}
    unsigned long timeSincePressed = millis() - tagEvent[oldestTag].tagTime;//This is the time since last scan.
    responseBuffer[6] = timeSincePressed >> 24; //MSB
    responseBuffer[7] = timeSincePressed >> 16; 
    responseBuffer[8] = timeSincePressed >> 8;
    responseBuffer[9] = timeSincePressed; //LSB
    if (oldestTag++ == MAX_TAG_STORAGE) oldestTag = 0;
  }
  else
  {
    //No new ID's. 
		for( int i = 0; i < 8; i++ )
		{
			responseBuffer[i] = 0; 
		}
  }
}

//Reads the current system settings from EEPROM
//If anything looks weird, reset setting to default value
void readSystemSettings(void)
{
  //Read what I2C address we should use
  setting_i2c_address = EEPROM.read(LOCATION_I2C_ADDRESS);
  if (setting_i2c_address == 255)
  {
    setting_i2c_address = I2C_ADDRESS_DEFAULT; //By default, we listen for I2C_ADDRESS_DEFAULT
    EEPROM.write(LOCATION_I2C_ADDRESS, setting_i2c_address);
  }

  else if( setting_i2c_address != I2C_ADDRESS_DEFAULT ) {
    EEPROM.write(LOCATION_I2C_ADDRESS, I2C_ADDRESS_DEFAULT);
  }
  
}

//Begin listening on I2C bus as I2C slave using the global variable setting_i2c_address
void startI2C()
{
  Wire.end(); //Before we can change addresses we need to stop
  
  if (digitalRead(addrPin) == HIGH) //Default is HIGH, the jumper is closed with paste in production
    Wire.begin(setting_i2c_address); //Start I2C and answer calls using address from EEPROM
  else
    Wire.begin(I2C_ADDRESS_NO_JUMPER); //Force address to I2C_ADDRESS_NO_JUMPER if user has opened the solder jumper

  //The connections to the interrupts are severed when a Wire.begin occurs. So re-declare them.
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}
