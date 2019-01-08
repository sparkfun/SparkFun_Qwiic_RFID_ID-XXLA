SparkX Qwiic RFID ID-XXLA
========================================

![SparkX Qwiic RFID](https://cdn.sparkfun.com/assets/parts/1/3/1/4/2/14867-Qwiic_RFID_ID-XXLA-06.jpg) 

[*SparX Qwiic RFID (SPX-14867)*](https://www.sparkfun.com/products/14867)

The Qwiic RFID ID-XXLA is a simple board that pairs with the ID-3LA, ID-12LA, ID-20LA, RFID modules. Included with the product is the ID-12LA and three RFID 125Khz cards as a full kit to get you started integrating an I2C RFID solution into your project. Utilizing the onboard ATtiny84A, the Qwiic RFID takes the six byte ID tag of your 125Khz RFID card, attaches a time stamp to it, and puts it onto a stack that holds up to 20 unique RFID scans at any given time. This information is easy to get at with some simple I2C commands and we’ve included some example code that shows you just how to do it using the broken out interrupt pin. We’ve also broken out two antennae pins that should only be used with the smaller ID-3LA.

The Qwiic RFID has a buzzer and LED that indicates that card has been scanned. We’ve included a jumper to disconnect the buzzer, a jumper for the I2C pull-up resistors, an interrupt jumper, and finally an address jumper. The Qwiic RFID comes fully decked out in Qwiic connectors, no need to solder a thing, just plug it in and start scanning!

We do not plan to regularly produce SparkX products so get them while they’re hot!

Repository Contents
-------------------

* **/Examples** - A number of examples to show how to read buttons with time, change the I2C address and scan for I2C devices
* **/Firmware** - The core sketch that runs Qwiic RFID ID-XXLA
* **/Hardware** - Eagle design files (.brd, .sch)

License Information
-------------------

This product is _**open source**_! 

Please review the LICENSE.md file for license information. 

If you have any questions or concerns on licensing, please contact techsupport@sparkfun.com.

Please use, reuse, and modify these files as you see fit. Please maintain attribution to SparkFun Electronics and release any derivative under the same license.

Distributed as-is; no warranty is given.

- Your friends at SparkFun.
