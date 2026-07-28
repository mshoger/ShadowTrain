#include "HttpServer.h"
#include <Arduino.h>
#include <WiFi.h>

HttpServer::HttpServer()
    : m_server(80)
{
}

void HttpServer::begin()
{
    Serial.printf("IP: %s\n", WiFi.localIP().toString().c_str());

    m_server.on("/", [this]()
    {
        handleRoot();
    });

    m_server.begin();

    Serial.println("HTTP server started");
}

void HttpServer::loop()
{
    m_server.handleClient();
}

void HttpServer::handleRoot()
{
    Serial.println("Received GET /");

    m_server.send(200, "text/plain", "Hello from ShadowTrain!");
}