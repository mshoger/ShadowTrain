#pragma once

#include <Arduino.h>
#include <esp_display_panel.hpp>

class Display
{
public:
    bool begin();

    uint16_t *framebuffer();

    int width() const { return 800; }
    int height() const { return 480; }

private:
    esp_panel::board::Board board;
    esp_panel::drivers::LCD *lcd = nullptr;
};