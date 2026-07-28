#include "Network.h"

WiFiManager::WiFiManager()
    : m_connected(false)
{
}

bool WiFiManager::begin(const char* ssid, const char* password)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    Serial.print("Connecting");

    int retries = 0;

    while (WiFi.status() != WL_CONNECTED && retries < 20)
    {
        delay(500);
        Serial.print(".");
        retries++;
    }

    m_connected = (WiFi.status() == WL_CONNECTED);

    if (m_connected)
    {
        Serial.println();
        Serial.println("Connected");
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
    }
    else
    {
        Serial.println();
        Serial.println("Connection failed");
    }

    return m_connected;
}

bool WiFiManager::connected() const
{
    return WiFi.status() == WL_CONNECTED;
}

String WiFiManager::ipAddress() const
{
    return WiFi.localIP().toString();
}

String WiFiManager::ssid() const
{
    return WiFi.SSID();
}

int WiFiManager::rssi() const
{
    return WiFi.RSSI();
}