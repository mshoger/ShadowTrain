#pragma once

#include <WebServer.h>

class HttpServer
{
public:
    HttpServer();

    void begin();
    void loop();

private:
    WebServer m_server;

    void handleRoot();
};