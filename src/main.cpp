#include <Arduino.h>

#include "app/Application.h"

Application app;

void setup()
{
    app.begin();
}

void loop()
{
    app.loop();
}