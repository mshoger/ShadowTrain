#include "HttpServer.h"
#include <WebServer.h>
#include <Arduino.h>
#include <WiFi.h>
#include <NetworkClient.h>
#include <cstring>

HttpServer::HttpServer()
    : m_server(80)
{
}

void HttpServer::begin(Display& display)
{
    m_display = &display;
    m_server.on("/", [this]()
    {
        handleRoot();
    });

    m_server.on("/status", [this]()
    {
        handleStatus();
    });

    m_server.on(
        "/frame",
        HTTP_POST,
        [this]()
        {
            handleFrame();
        },
        [this]()
        {
            handleFrameUpload();
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
    json += "\"height\":480,";
    json += "\"framebuffer\":";
    json += (m_display && m_display->framebuffer()) ? "true" : "false";
    json += "}";

    json += "}";

    m_server.send(200, "application/json", json);
}

void HttpServer::handleFrame()
{
    m_server.send(200, "text/plain", "OK");
}

void HttpServer::handleFrameUpload()
{
    HTTPUpload& upload = m_server.upload();

    switch (upload.status)
    {
    case UPLOAD_FILE_START:
{
    m_frameOffset = 0;
    Serial.println("Frame upload started");
    break;
}

case UPLOAD_FILE_WRITE:
{
    uint16_t* framebuffer = m_display->framebuffer();

    if (!framebuffer)
        break;

    constexpr size_t FRAMEBUFFER_SIZE = 800 * 480 * 2;

    if (m_frameOffset + upload.currentSize > FRAMEBUFFER_SIZE)
    {
        Serial.println("Upload exceeds framebuffer size!");
        break;
    }

    memcpy(
        reinterpret_cast<uint8_t*>(framebuffer) + m_frameOffset,
        upload.buf,
        upload.currentSize);

    m_frameOffset += upload.currentSize;

    break;
}

    case UPLOAD_FILE_END:
        Serial.printf("Frame upload complete: %u bytes\n", upload.totalSize);
        break;

    case UPLOAD_FILE_ABORTED:
        Serial.println("Frame upload aborted");
        break;

    default:
        break;
    }
}