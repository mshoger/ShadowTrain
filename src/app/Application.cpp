#include "Application.h"

#include "../assets/ShadowTrainLogo.h"
#include <Config.h>

Application::Application()
{
}

namespace
{
    constexpr int LEFT_MARGIN = 20;

    constexpr int LOGO_TOP = 40;

    constexpr int TITLE_Y = 200;

    constexpr int STATUS_Y = 250;
    constexpr int LINE_HEIGHT = 20;

    constexpr int IP_Y = 330;
    constexpr int IP_X = 60;
}

void Application::begin()
{
    //
    // Initialize hardware
    //
    Serial.begin(115200);
    delay(1000);

    Serial.println("ShadowTrain Viewer starting...");

    display.begin();

    graphics.begin(
        display.framebuffer(),
        display.width(),
        display.height());

    //
    // Show the splash screen immediately
    //
    showSplash();

    //
    // Connect to Wi-Fi
    //
    if (connectToWiFi())
    {
        //
        // Start HTTP server
        //
        server.begin();

        graphics.drawString(
            LEFT_MARGIN,
            STATUS_Y + LINE_HEIGHT * 2,
            "Server  : Running      ",
            Colors::Green);

        Serial.println("HTTP server started.");
    }
    else
    {
        graphics.drawString(
            LEFT_MARGIN,
            STATUS_Y + LINE_HEIGHT * 2,
            "Server  : Offline      ",
            Colors::Red);

        Serial.println("HTTP server not started.");
    }
}

void Application::loop()
{
    //
    // Only service the HTTP server while connected.
    // Later we'll add automatic reconnect logic here.
    //
    if (network.connected())
    {
        server.loop();
    }

    //
    // Future work:
    //
    // - Automatic Wi-Fi reconnect
    // - Touch handling
    // - Image reception
    // - UI updates
    //
}

void Application::showSplash()
{
    graphics.clear(Colors::Black);

    //
    // Draw logo
    //
    int logoX = (display.width() - ShadowTrainLogoWidth) / 2;

    graphics.drawImage(
        logoX,
        LOGO_TOP,
        ShadowTrainLogoWidth,
        ShadowTrainLogoHeight,
        ShadowTrainLogo);

    graphics.drawString(
        LEFT_MARGIN,
        TITLE_Y,
        "ShadowTrain Viewer",
        Colors::White);

    graphics.drawString(
        LEFT_MARGIN,
        STATUS_Y,
        "Display : OK",
        Colors::Green);

    graphics.drawString(
        LEFT_MARGIN,
        STATUS_Y + LINE_HEIGHT,
        "Wi-Fi   : Connecting...",
        Colors::Yellow);

    graphics.drawString(
        LEFT_MARGIN,
        STATUS_Y + LINE_HEIGHT * 2,
        "Server  : Waiting...",
        Colors::Yellow);

    graphics.drawString(
        LEFT_MARGIN,
        STATUS_Y + LINE_HEIGHT * 3,
        "Version : 0.1.0",
        Colors::Gray);
}

bool Application::connectToWiFi()
{
    if (network.begin(WIFI_SSID, WIFI_PASSWORD))
    {
        graphics.drawString(
            LEFT_MARGIN,
            STATUS_Y + LINE_HEIGHT,
            "Wi-Fi   : Connected      ",
            Colors::Green);

        graphics.drawString(
            LEFT_MARGIN,
            IP_Y,
            "IP:",
            Colors::White);

        graphics.drawString(
            IP_X,
            IP_Y,
            network.ipAddress().c_str(),
            Colors::Cyan);

        return true;
    }

    graphics.drawString(
        LEFT_MARGIN,
        STATUS_Y + LINE_HEIGHT,
        "Wi-Fi   : FAILED         ",
        Colors::Red);

    return false;
}