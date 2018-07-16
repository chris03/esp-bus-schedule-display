#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <WebSocketsServer.h>
#include <FS.h>
#include <WiFiManager.h>

const char JSON_CMD[] = "json:";

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
ESP8266HTTPUpdateServer updateServer(true /* Serial Debug */);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    Serial.printf("webSocketEvent(%d, %d, ...)\r\n", num, type);
    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.printf("[%u] Disconnected!\r\n", num);
        break;
    case WStype_CONNECTED:
    {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\r\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // Send msg
        const char msg[] = "Hello";
        webSocket.sendTXT(num, msg, strlen(msg));
    }
    break;
    case WStype_TEXT:
        Serial.printf("[%u] get Text: %s\r\n", num, payload);

        if (strcmp("refresh", (const char *)payload) == 0)
        {
            nextDraw = 1;
        }
        else if (strcmp("clearWifi", (const char *)payload) == 0)
        {
            WiFiManager wifiManager;
            wifiManager.resetSettings();
            ESP.restart();
        }
        else if (strncmp((const char *)payload, JSON_CMD, strlen(JSON_CMD)) == 0)
        {
            webSocket.sendTXT(num, "Saving new config...");
            const char *jsonConfig = (const char *)payload + strlen(JSON_CMD);

            if (saveConfig(jsonConfig))
            {
                webSocket.sendTXT(num, "Saved! Reloading config...");

                loadBusConfig();

                nextDraw = 1;
            }
            else
            {
                webSocket.sendTXT(num, "Failed!");
            }
        }
        else
        {
            Serial.println("Unknown command");
        }

        // webSocket.broadcastTXT(payload, length);
        break;
    default:
        Serial.printf("Invalid WStype [%d]\r\n", type);
        break;
    }
}

void setupWeb()
{
    Serial.println("Configuring web server...");

    server.serveStatic("/", SPIFFS, "/index.html");

    server.on("/config", []() {
        server.send(200, "application/json; charset=utf-8", loadConfig());
    });

    server.onNotFound([]() {
        server.send(404, "text/plain", "Not Found");
    });

    updateServer.setup(&server);

    server.begin();
    Serial.println("HTTP server started");

    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    Serial.println("WebSocket started");
}

void handleWeb()
{
    webSocket.loop();
    server.handleClient();
}
