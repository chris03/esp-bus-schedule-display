# Bus Schedule Display

[![Build Status](https://travis-ci.org/chris03/esp-bus-schedule-display.svg?branch=master)](https://travis-ci.org/chris03/esp-bus-schedule-display)

## Purpose

Show in realtime the minutes remaining of the upcoming buses  at a given stop using the NextBus API.

## Hardware
ESP8266  
ST7735S 1.8 Inch SPI TFT Display 

## Features

* Call NextBus API, parse and display the predicted arrival times
* Web page to configure:
  * Bus agency and stop id
  * Shown time interval (hide buses more that X minutes away)
  * Ignored bus lines (Do not show their schedule)
  * Text color depending on the time remaining (Like green when smaller than 10min.)
* Auto wifi config (go into hotspot mode when no wifi config)
* Web and OTA updates enabled
 

## Wiring
```
    Display                ESP
-----------------------------------------
CS   (SD card) <=>
MOSI (SD card) <=> 
MISO (SD card) <=>
SCK  (SD card) <=>
CS             <=> CS   GPIO15 (configurable)
SCL            <=> CLK  GPIO14
SDA            <=> MOSI GPIO13
A0             <=> DC   GPIO4  (configurable)
RESET          <=>      GPIO5  (configurable)

See build_flags in platformio.ini for configurable pins.
``` 

## External libraries

https://github.com/tzapu/WiFiManager
https://github.com/Bodmer/TFT_eSPI
https://github.com/arduino-libraries/NTPClient.git
https://github.com/bblanchon/ArduinoJson.git
https://github.com/Links2004/arduinoWebSockets.git

## External API

https://www.nextbus.com/xmlFeedDocs/NextBusXMLFeed.pdf