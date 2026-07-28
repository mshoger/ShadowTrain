#include "Display.h"

using namespace esp_panel::board;

bool Display::begin()
{
    if (!board.begin())
        return false;

    lcd = board.getLCD();

    return lcd != nullptr;
}

uint16_t *Display::framebuffer()
{
    if (!lcd)
        return nullptr;

    return static_cast<uint16_t *>(lcd->getFrameBufferByIndex(0));
}