# Bus Schedule Display

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
CS             <=> CS   GPIO15 (configurable, see code)
SCL            <=> CLK  GPIO14
SDA            <=> MOSI GPIO13
A0             <=> DC   GPIO4  (configurable)
RESET          <=>      GPIO5  (configurable)
``` 