#include "HttpServer.h"
#include <Arduino.h>
#include <WiFi.h>

HttpServer::HttpServer()
    : m_server(80)
{
}

void HttpServer::begin()
{
    m_server.on("/", [this]()
    {
        handleRoot();
    });

    m_server.on("/status", [this]()
    {
        handleStatus();
    });

    m_server.begin();

    Serial.printf("HTTP server listening on http://%s/\n",
                  WiFi.localIP().toString().c_str());
}

void HttpServer::loop()
{
    m_server.handleClient();
}

void HttpServer::handleRoot()
{
        m_server.send(200, "text/plain", "Hello from ShadowTrain!");
}

void HttpServer::handleStatus()
{
    String json;

    json += "{";
    json += "\"device\":\"ShadowTrain Viewer\",";
    json += "\"version\":\"0.2.0\",";
    json += "\"ip\":\"" + WiFi.localIP().toString() + "\",";
    json += "\"ssid\":\"" + WiFi.SSID() + "\",";
    json += "\"rssi\":" + String(WiFi.RSSI());
    json += "}";

    m_server.send(200, "application/json", json);
}