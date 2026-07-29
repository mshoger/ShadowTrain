#include "HttpServer.h"
#include <WebServer.h>
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

    m_server.on("/frame", HTTP_POST, [this]()
    {
        handleFrame();
    });

    m_server.onNotFound([this]()
    {
        m_server.send(404, "text/plain", "Not Found");
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
    m_server.sendHeader("Location", "/status");
    m_server.send(302, "text/plain", "");
}

void HttpServer::handleStatus()
{
    String json;

    json += "{";

    json += "\"device\":\"ShadowTrain Viewer\",";
    json += "\"version\":\"0.2.0\",";

    json += "\"wifi\":{";
    json += "\"connected\":";
    json += (WiFi.status() == WL_CONNECTED) ? "true" : "false";
    json += ",";
    json += "\"ssid\":\"";
    json += WiFi.SSID();
    json += "\",";
    json += "\"ip\":\"";
    json += WiFi.localIP().toString();
    json += "\",";
    json += "\"rssi\":";
    json += String(WiFi.RSSI());
    json += "},";

    json += "\"display\":{";
    json += "\"width\":800,";
    json += "\"height\":480";
    json += "}";

    json += "}";

    m_server.send(200, "application/json", json);
}

void HttpServer::handleFrame()
{
    String response;

    response += "Content-Length: ";
    response += String(m_server.clientContentLength());

    m_server.send(200, "text/plain", response);
}