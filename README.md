# ESP8266.XmasTreeCandles
Control Tchibo Wireless LED Xmas Tree Candles via Amazon Echo (Alexa)

s. [https://www.tchibo.de/10-led-weihnachtsbaumkerzen-p400093456.html]

## Description
I created this small arduino sketch for controlling wireless LED Xmas Tree candles from Tchibo via Alexa for private use. So, first I read out the sequences the IR remote control sends by clicking ON or OFF. For this see the samples at [IRremote library for ESP8266](https://github.com/markszabo/IRremoteESP8266). After that I used [FauxmoESP](https://bitbucket.org/xoseperez/fauxmoesp), so my ESP8266 now acts as a wireless switch by using the Philips Hue protocol. Together with the sequences from my Tchibo remote control I can now remotely turn on or off the Xmas Tree Candles. 

(see [IRremote wiki](https://github.com/markszabo/IRremoteESP8266/wiki) for wiring)

## Requirements (Hardware)

* ESP8266-12E Microcontroller (NodeMCU)
* 2N3904 Transistor
* 940nm IR LED

(ESP-01 is also possible, but a bit more elaborate)

## Requirements (Software)

* [Arduino IDE](http://arduino.cc)
* [ESP8266 Library for Arduino IDE](https://github.com/esp8266/Arduino)
* [IRremote library for ESP8266](https://github.com/markszabo/IRremoteESP8266)
* [FauxmoESP](https://bitbucket.org/xoseperez/fauxmoesp)

## Warrenty
I do not guarantee that the source code works with all Tchibo LED candles. Also, I do not take warranty of any damage that may be caused by the source code on the microcontroller, or the LED candles.
