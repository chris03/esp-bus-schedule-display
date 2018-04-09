#ifndef CHRIS03_TIME_H
#define CHRIS03_TIME_H

#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

namespace Chris03
{
namespace Time
{
WiFiUDP udp;
NTPClient ntpClient(udp, "ca.pool.ntp.org", -5 * 60 * 60);

const uint8_t BufferSize = 6;
char Buffer[BufferSize];

String getTime()
{
    snprintf(Buffer, BufferSize, "%02d:%02d", ntpClient.getHours(), ntpClient.getMinutes());
    return String(Buffer);
}
}
}
#endif