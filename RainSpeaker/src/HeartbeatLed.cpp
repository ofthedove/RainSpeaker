#include "HeartbeatLed.h"
#include <Arduino.h>

enum
{
    taskStackSize = 10000,
};

void HeartbeatLedTask(void *parameter)
{
    volatile HeartbeatLed *instance = (HeartbeatLed *)parameter;

    while (true)
    {
        switch (instance->pattern)
        {
        case HeartbeatLedPattern::solidOn:
            break;

        case HeartbeatLedPattern::blink500:
            instance->state = !instance->state;
            digitalWrite(instance->pin, !instance->state);
            break;
        }

        delay(250);
    }
}

HeartbeatLed::HeartbeatLed(int pin)
{
    this->pin = pin;
    this->pattern = HeartbeatLedPattern::init;
    this->state = false;

    pinMode(pin, OUTPUT);

    xTaskCreate(HeartbeatLedTask, "Heartbeat LED Task", taskStackSize, this, 5, &HeartbeatLedTaskHandle);
}

void HeartbeatLed::SetPattern(HeartbeatLedPattern pattern)
{
    this->pattern = pattern;
    switch (pattern)
    {
    case HeartbeatLedPattern::solidOn:
        state = true;
        digitalWrite(pin, !state);
    case HeartbeatLedPattern::blink500:
        state = !state;
        digitalWrite(pin, !state);
        break;
    }
}
