#include "Graphics.h"

#include "../font/Font.h"
#include "../font/font8x8_basic.h"

#include <algorithm>
#include <cstring>

Graphics::Graphics()
    : m_framebuffer(nullptr),
      m_width(0),
      m_height(0)
{
}

void Graphics::begin(uint16_t *framebuffer, int width, int height)
{
    m_framebuffer = framebuffer;
    m_width = width;
    m_height = height;
}

int Graphics::width() const
{
    return m_width;
}

int Graphics::height() const
{
    return m_height;
}

uint16_t *Graphics::framebuffer()
{
    return m_framebuffer;
}

void Graphics::clear(uint16_t color)
{
    if (!m_framebuffer)
        return;

    std::fill_n(
        m_framebuffer,
        m_width * m_height,
        color);
}

void Graphics::drawPixel(int x, int y, uint16_t color)
{
    if (!m_framebuffer)
        return;

    if (x < 0 || x >= m_width)
        return;

    if (y < 0 || y >= m_height)
        return;

    m_framebuffer[y * m_width + x] = color;
}

void Graphics::fillRect(int x, int y, int w, int h, uint16_t color)
{
    if (!m_framebuffer)
        return;

    if (w <= 0 || h <= 0)
        return;

    int x0 = max(0, x);
    int y0 = max(0, y);
    int x1 = min(m_width, x + w);
    int y1 = min(m_height, y + h);

    for (int yy = y0; yy < y1; yy++)
    {
        uint16_t *row = m_framebuffer + yy * m_width;

        for (int xx = x0; xx < x1; xx++)
        {
            row[xx] = color;
        }
    }
}

void Graphics::drawImage(
    int x,
    int y,
    int width,
    int height,
    const uint16_t *pixels)
{
    if (!m_framebuffer || !pixels)
        return;

    // Fast path - image is completely on screen
    if (x >= 0 &&
        y >= 0 &&
        x + width <= m_width &&
        y + height <= m_height)
    {
        for (int yy = 0; yy < height; yy++)
        {
            std::memcpy(
                &m_framebuffer[(y + yy) * m_width + x],
                &pixels[yy * width],
                width * sizeof(uint16_t));
        }

        return;
    }

    // Slow path - clipped image
    for (int yy = 0; yy < height; yy++)
    {
        int dstY = y + yy;

        if (dstY < 0 || dstY >= m_height)
            continue;

        uint16_t *dst = m_framebuffer + dstY * m_width;

        for (int xx = 0; xx < width; xx++)
        {
            int dstX = x + xx;

            if (dstX < 0 || dstX >= m_width)
                continue;

            dst[dstX] = pixels[yy * width + xx];
        }
    }
}

void Graphics::drawChar(
    int x,
    int y,
    char c,
    uint16_t foreground,
    uint16_t background)
{
    if ((unsigned char)c > 127)
        return;

    for (int row = 0; row < Font::Height; row++)
    {
        uint8_t bits = font8x8_basic[(uint8_t)c][row];

        for (int col = 0; col < Font::Width; col++)
        {
            if (bits & (1 << col))
                drawPixel(x + col, y + row, foreground);
            else
                drawPixel(x + col, y + row, background);
        }
    }
}

void Graphics::drawString(
    int x,
    int y,
    const char *text,
    uint16_t foreground,
    uint16_t background)
{
    while (*text)
    {
        drawChar(
            x,
            y,
            *text,
            foreground,
            background);

        x += Font::Width + 1; // Add 1 pixel of spacing between characters
        ++text;
    }
}