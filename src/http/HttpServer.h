#pragma once

#include <WebServer.h>
#include "../display/Display.h"

class HttpServer
{
public:
    HttpServer();

    void begin(Display& display);
    void loop();

private:
    WebServer m_server;
    Display* m_display = nullptr;

    size_t m_frameOffset = 0;

    void handleRoot();
    void handleStatus();
    void handleFrame();
    void handleFrameUpload();
};