#pragma once

#include "../display/Display.h"
#include "../graphics/Graphics.h"
#include "../network/Network.h"

class Application
{
public:
    Application();

    void begin();
    void loop();

private:
    void showSplash();
    void connectToWiFi();

    Display display;
    Graphics graphics;
    WiFiManager network;
};