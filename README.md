SparkFun Qwiic RFID ID-XXLA
========================================

<table class="table table-hover table-striped table-bordered">
  <tr align="center">
   <td><a href="https://www.sparkfun.com/products/15191"><img src="https://cdn.sparkfun.com//assets/parts/1/3/6/1/0/15191-SparkFun_RFID_Qwiic_Reader-01a.jpg" alt="RFID Qwiic Reader"></a></td>
   <td><a href="https://www.sparkfun.com/products/15209"><img src="https://cdn.sparkfun.com//assets/parts/1/3/6/4/0/15209-SparkFun_RFID_Qwiic_Kit-01a.jpg" alt="RFID Qwiic Kit"></a></td>
  </tr>
  <tr align="center">
    <td><i>SparkFun RFID Qwiic Reader [<a href="https://www.sparkfun.com/products/15191">SEN-15191</a>]</i></td>
    <td><i>SparkFun RFID Qwiic Reader Kit [<a href="https://www.sparkfun.com/products/15209">KIT-15209</a>]<i></td>
  </tr>
</table>
     
The SparkFun Qwiic RFID ID-XXLA is a simple board that pairs with the ID-3LA, ID-12LA, ID-20LA, RFID modules.Utilizing the onboard ATtiny84A, the Qwiic RFID takes the six byte ID tag of your 125Khz RFID card, attaches a time stamp to it, and puts it onto a stack that holds up to 20 unique RFID scans at any given time. This information is easy to get at with some simple I2C commands and we’ve included some example code that shows you just how to do it using the broken out interrupt pin. We’ve also broken out two antennae pins that should only be used with the smaller ID-3LA.

The SparkFun Qwiic RFID has a buzzer and LED that indicates that card has been scanned. We’ve included a jumper to disconnect the buzzer, a jumper for the I2C pull-up resistors, an interrupt jumper, and finally an address jumper. The Qwiic RFID comes fully decked out in Qwiic connectors, no need to solder a thing, just plug it in and start scanning!

Repository Contents
-------------------

* **/Firmware** - The core sketch that runs the SparkFun Qwiic RFID ID-XXLA
* **/Arduino Examples** - A number of examples to show how to read buttons with time, change the I2C address and scan for I2C devices
* **/ATtiny Firmware** - Default firmware for the Qwiic RFID
* **/Hardware** - Eagle design files (.brd, .sch)

Documentation
--------------

* **[Hookup Guide](https://learn.sparkfun.com/tutorials/sparkfun-qwiic-rfid-idxxla-hookup-guide)** - Basic hookup guide.

Arduino Library 
----------------
* **[SparkFun Qwiic RFID Arduino Library](https://github.com/sparkfun/SparkFun_Qwiic_RFID_Arduino_Library)**

Python Package
----------------
* **[SparkFun Qwiic RFID Python Package](https://github.com/sparkfun/Qwiic_RFID_Py)**

Product Versions
----------------
* [SEN-15191](https://www.sparkfun.com/products/15191)- SparkFun Qwiic RFID ID-XXLA
* [KIT-15209](https://www.sparkfun.com/products/15209)- SparkFun Qwiic RFID ID-XXLA with ID Innovations RFID Module, Qwiic Cable, and RFID Tagged Cards, 

License Information
-------------------

This product is _**open source**_! 

Please review the LICENSE.md file for license information. 

If you have any questions or concerns on licensing, please contact techsupport@sparkfun.com.

Please use, reuse, and modify these files as you see fit. Please maintain attribution to SparkFun Electronics and release any derivative under the same license.

Distributed as-is; no warranty is given.

- Your friends at SparkFun.
