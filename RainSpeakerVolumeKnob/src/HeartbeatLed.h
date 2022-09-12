#ifndef HEARTBEATLED_H
#define HEARTBEATLED_H

#include <Arduino.h>

enum class HeartbeatLedPattern
{
    solidOn,
    blink500,
    init = blink500,
};

class HeartbeatLed
{
public:
    HeartbeatLed(int pin);
    void SetPattern(HeartbeatLedPattern pattern);

    // private:
    int pin;
    HeartbeatLedPattern pattern;
    bool state;
    TaskHandle_t HeartbeatLedTaskHandle;
};

#endif
