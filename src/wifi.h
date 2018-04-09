#include <ESP8266WiFi.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

void configModeCallback(WiFiManager *myWiFiManager)
{
    Serial.println("Entered config mode");
    Serial.println(WiFi.softAPIP());

    display.printCenter("Connect to " + myWiFiManager->getConfigPortalSSID(), 1);
    Serial.println(myWiFiManager->getConfigPortalSSID());
}

void setupWifi()
{
    //WiFiManager
    WiFiManager wifiManager;
    wifiManager.setAPCallback(configModeCallback);

    // Reset saved settings
    //wifiManager.resetSettings();

    if (!wifiManager.autoConnect())
    {
        Serial.println("failed to connect and hit timeout");
        display.printCenter("Failed to connect", 1);
        delay(2000);
        ESP.reset();
        delay(1000);
    }

    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(WiFi.SSID());
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    display.printCenter("Connected to: " + WiFi.SSID(), 1);
    delay(4000);
    display.printCenter("IP: " + WiFi.localIP().toString(), 1);

    // OTA
    //ArduinoOTA.setPort(8266);
    //ArduinoOTA.setHostname("esp1");
    // ArduinoOTA.setPassword((const char *)"123");

    ArduinoOTA.onStart([]() {
        Serial.println("OTA Start");
        display.clear();
        display.printCenter("Updating...", 1);
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nOTA End");
        display.printCenter("Update done!", 1);
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        unsigned int percent = (progress / (total / 100));
        display.printCenter("Updating " + String(percent) + String("%"), 1);
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR)
            Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR)
            Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR)
            Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR)
            Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR)
            Serial.println("End Failed");

        display.printCenter("Update failed!", 1);
        delay(2000);
        Serial.println("Restarting...");
        ESP.restart();
    });
    ArduinoOTA.begin();
}

void handleOTA()
{
    ArduinoOTA.handle();
}
