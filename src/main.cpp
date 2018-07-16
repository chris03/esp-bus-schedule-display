
#include <Arduino.h>

#include "BusPredictions.h"
Chris03::BusPredictionsConfig busConfig;
Chris03::BusPredictions busPredictions(busConfig);

#include "Display.h"
Chris03::Display display;

const char PIN_BACKLIGHT_POWER = 16;
const char PIN_BTN1 = 0;
const char PIN_BTN2 = 2;

unsigned long nextDraw = 0;
unsigned long nextTimeDraw = 0;
unsigned long nextBacklightOff = 0;

#include "NetworkTime.h"
#include "wifi.h"
#include "config.h"
#include "WebServer.h"

void drawBusPredictions(std::vector<Chris03::BusPredictionItem> &busPredictions)
{
  const int MAX_PREDICTIONS_DISPLAY = 3;
  const int MAX = busPredictions.size() < MAX_PREDICTIONS_DISPLAY ? busPredictions.size() : MAX_PREDICTIONS_DISPLAY;

  display.clear();

  for (int i = 0; i < MAX; ++i)
  {
    if (busPredictions[i].minutes <= 1)
    {
      display.setTextColor(TFT_RED);
    }
    else if (busPredictions[i].minutes <= 5)
    {
      display.setTextColor(TFT_ORANGE);
    }
    else if (busPredictions[i].minutes <= 10)
    {
      display.setTextColor(TFT_GREEN);
    }
    else
    {
      display.setTextColor(TFT_WHITE);
    }

    display.tft->setTextFont(4);
    String minutesStr = String(busPredictions[i].minutes);
    int w = display.tft->textWidth(minutesStr);
    int y = display.tft->getCursorY();
    display.tft->drawString(minutesStr, 32 - w, y);

    display.tft->setTextFont(1);
    display.tft->drawString("min.", 34, y + 12);

    display.setTextColor(TFT_WHITE);

    display.tft->setTextFont(4);
    display.tft->setCursor(70, y);
    display.tft->println(busPredictions[i].routeTag);

    display.tft->setTextFont(1);
    display.tft->println(busPredictions[i].routeTitle);
    display.tft->drawFastHLine(0, display.tft->getCursorY() + 1, display.tft->width(), TFT_WHITE);
    display.tft->setCursor(display.tft->getCursorX(), display.tft->getCursorY() + 4);
  }
}

void setup()
{
  busPredictions.setOnUpdateCallback(drawBusPredictions);

  Serial.begin(115200);

  // Enable display
  pinMode(PIN_BACKLIGHT_POWER, OUTPUT);
  digitalWrite(PIN_BACKLIGHT_POWER, HIGH);

  pinMode(PIN_BTN1, INPUT_PULLUP);
  pinMode(PIN_BTN2, INPUT_PULLUP);

  nextBacklightOff = millis() + 60000;

  display.init(1 /* Rotatation */);
  display.printCenter("Bus Schedule Display", 2);
  setupWifi();
}

void loop()
{
  unsigned long now = millis();

  if (digitalRead(PIN_BTN2) == LOW)
  {
    Serial.println("Button on GPIO2 pressed");

    // Power on backlight for 30 sec.
    digitalWrite(PIN_BACKLIGHT_POWER, HIGH);
    nextBacklightOff = millis() + 30000;
  }

  if (digitalRead(PIN_BTN1) == LOW)
  {
    Serial.println("Button on GPIO0 pressed");
  }

  if (nextBacklightOff < now)
  {
    // Power down backlight
    digitalWrite(PIN_BACKLIGHT_POWER, LOW);
  }

  // First run
  if (nextDraw == 0)
  {
    if (!SPIFFS.begin())
    {
      Serial.println("SPIFFS initialisation failed!");
    }

    setupWeb();
    loadBusConfig();
    Chris03::Time::ntpClient.begin();

    // Delay 2 sec. to show IP address
    nextTimeDraw = nextDraw = now + 2 * 1000;
  }

  handleOTA();
  handleWeb();
  Chris03::Time::ntpClient.update();

  if (nextDraw < now)
  {
    busPredictions.update();
    nextDraw = now + busConfig.refreshIntervalSeconds * 1000;
    nextTimeDraw = 0;
  }

  if (nextTimeDraw < now)
  {
    display.printCenterBottom(Chris03::Time::getTime(), 2);
    nextTimeDraw = now + 1000;
  }
}
