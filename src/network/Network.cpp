#include "Network.h"

WiFiManager::WiFiManager()
    : m_connected(false)
{
}

bool WiFiManager::begin(const char* ssid, const char* password)
{
    WiFi.mode(WIFI_STA);
    WiFi.setHostname("shadowviewer");

    Serial.println("Scanning for networks...");

    int n = WiFi.scanNetworks();

    if (n == 0)
    {
        Serial.println("No networks found.");
    }
    else
    {
        Serial.printf("Found %d network(s):\n", n);

        for (int i = 0; i < n; i++)
        {
            Serial.printf("%2d: %-32s RSSI=%4d Ch=%2d %s\n",
                          i + 1,
                          WiFi.SSID(i).c_str(),
                          WiFi.RSSI(i),
                          WiFi.channel(i),
                          (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "Open" : "Secured");
        }
    }

    Serial.printf("\nConnecting to '%s'...\n", ssid);

    WiFi.begin(ssid, password);

    int retries = 20;

    while (WiFi.status() != WL_CONNECTED && retries-- > 0)
    {
        Serial.print(".");
        delay(500);
    }

    Serial.println();

    if (WiFi.status() == WL_CONNECTED)
{
    m_connected = true;

    Serial.println("Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    if (MDNS.begin("shadowviewer"))
    {
        Serial.println("mDNS responder started");
    }
    else
    {
        Serial.println("Failed to start mDNS");
    }

    return true;
}

    m_connected = false;

    Serial.printf("Connection failed. WiFi.status() = %d\n", WiFi.status());

    return false;
}

bool WiFiManager::connected() const
{
    return m_connected && WiFi.status() == WL_CONNECTED;
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