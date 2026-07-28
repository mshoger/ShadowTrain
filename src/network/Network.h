#pragma once

#include <Arduino.h>
#include <WiFi.h>

class WiFiManager
{
public:
    WiFiManager();

    bool begin(const char* ssid, const char* password);

    bool connected() const;

    String ipAddress() const;

    String ssid() const;

    int rssi() const;

private:
    bool m_connected;
};