
DMX512 to EV3 Brick bridge
===

This software is a bridge between a [DMX512 bus](https://en.wikipedia.org/wiki/DMX512) and an [Lego EV3 Mindstorms brick](https://en.wikipedia.org/wiki/Lego_Mindstorms_EV3).

In terms of hardware, it uses a [SparkFun ESP32 DMX to LED Shield](https://www.sparkfun.com/products/15110) and a [SparkFun Thing Plus - ESP32](https://www.sparkfun.com/products/15663).

It works as follows:

* Registers on the DMX bus as a receiver. The channel it listens to is hardcoded in the source code (default 127).
* If it detects a value above or below a certain threshold (127) it writes a HIGH or LOW value to an output pin D2 of the DMX shield. This corresponds to pin 27 of the ESP32 device.
* All detected transitions on the DMX bus are written to the EV3 shield as described above as well an on the ESP32 serial port.

License
===
[Apache 2.0](LICENSE.txt)
