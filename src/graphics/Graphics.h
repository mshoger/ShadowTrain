#pragma once

#include <Arduino.h>

namespace Colors
{
    constexpr uint16_t Black   = 0x0000;
    constexpr uint16_t White   = 0xFFFF;

    constexpr uint16_t Red     = 0xF800;
    constexpr uint16_t Green   = 0x07E0;
    constexpr uint16_t Blue    = 0x001F;

    constexpr uint16_t Yellow  = 0xFFE0;
    constexpr uint16_t Cyan    = 0x07FF;
    constexpr uint16_t Magenta = 0xF81F;

    constexpr uint16_t Gray    = 0x8410;
}

class Graphics
{
public:
    Graphics();

    void begin(uint16_t *framebuffer, int width, int height);

    int width() const;
    int height() const;

    void clear(uint16_t color);
    void drawPixel(int x, int y, uint16_t color);
    void fillRect(int x, int y, int w, int h, uint16_t color);

    void drawImage(
        int x,
        int y,
        int width,
        int height,
        const uint16_t *pixels);

    uint16_t *framebuffer();

    void drawChar(
        int x,
        int y,
        char c,
        uint16_t foreground,
        uint16_t background = Colors::Black);

    void drawString(
        int x,
        int y,
        const char *text,
        uint16_t foreground,
        uint16_t background = Colors::Black);

private:
    uint16_t *m_framebuffer = nullptr;
    int m_width = 0;
    int m_height = 0;
};