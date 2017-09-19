# busputer

The BusPuter project is an bus based bord computer for cars.

This Project based on an Adafruit Feather M0 with a Atmel 32Bit ARM CPU and additional a SIM808 based module. 
To communicate with the BusPuter we use the [BLYNK](http://www.blynk.cc) Project and the [TinyGSM](https://github.com/vshymanskyy/TinyGSM) library.
For displays we use the [U8G2](https://github.com/olikraus/u8g2) library to support some displays.

Following functions are integrated:
* Time sync via GPS
* Speed via GPS
* Position tracking via Blynk (GPRS)
* Bord voltage
* Dimming LCD background with cluster dimmer
* Logging on a SD Card
* Water temperature (optional)
* Fuel level (optional)
* RPM (optional)
* Speed via VW GALA or Speedpulse (optional)

It's also possible to add more function over an I2C bus.

See German Forum:

http://www.bulliforum.com/viewtopic.php?f=1&t=96052

http://www.vwbus-online.org/forum/forum_entry.php?id=1626183
