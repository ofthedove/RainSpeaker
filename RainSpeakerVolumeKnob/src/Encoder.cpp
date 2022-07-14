#include "Encoder.h"
#include <Arduino.h>

enum
{
    buttonPin = 5,
    encoderDtPin = 18,
    encoderClkPin = 19,

    buttonToggleDebounceMillis = 20,
    encoderQueueSize = 100,
};

static const short int EncoderArray[4][4] =
    {{0, 1, -1, 0},
     {-1, 0, 0, 1},
     {1, 0, 0, -1},
     {0, -1, 1, 0}};

static volatile uint8_t encoderPreviousValue = 0;
static uint8_t encoderPosition = 0;

static volatile bool buttonToggleValue = false;
static volatile unsigned long buttonToggleOldMillis;

static volatile uint8_t circularQueue[encoderQueueSize] = {0};
static volatile uint8_t head = 0;
static volatile uint8_t tail = 0;

void IRAM_ATTR ButtonChangeIsr()
{
    if (digitalRead(buttonPin) == LOW && millis() - buttonToggleOldMillis > buttonToggleDebounceMillis)
    {
        buttonToggleValue = !buttonToggleValue;
        buttonToggleOldMillis = millis();
    }
}

void IRAM_ATTR EncoderChangeIsr()
{
    byte encoderValue = (digitalRead(encoderDtPin) << 1) + digitalRead(encoderClkPin);

    circularQueue[head] = encoderValue;
    head++;
    if (head == encoderQueueSize)
    {
        head = 0;
    }
}

Encoder &Encoder::getInstance()
{
    static Encoder instance;

    return instance;
}

void Encoder::Init()
{
    detachInterrupt(buttonPin);
    detachInterrupt(encoderDtPin);
    detachInterrupt(encoderClkPin);

    encoderPreviousValue = 0;
    encoderPosition = 0;
    head = 0;
    tail = 0;

    buttonToggleValue = false;

    pinMode(buttonPin, INPUT);
    pinMode(encoderDtPin, INPUT);
    pinMode(encoderClkPin, INPUT);

    buttonToggleOldMillis = millis();

    attachInterrupt(buttonPin, ButtonChangeIsr, CHANGE);
    attachInterrupt(encoderDtPin, EncoderChangeIsr, CHANGE);
    attachInterrupt(encoderClkPin, EncoderChangeIsr, CHANGE);
}

void Encoder::Run()
{
    while (tail != head)
    {
        short unsigned int encoderValue = circularQueue[tail];
        encoderPosition += EncoderArray[encoderPreviousValue][encoderValue];
        encoderPreviousValue = encoderValue;
        tail++;
        if (tail == encoderQueueSize)
        {
            tail = 0;
        }
    }
}

uint8_t Encoder::GetPosition()
{
    return encoderPosition;
}

void Encoder::SetPosition(uint8_t position)
{
    encoderPosition = position;
}

bool Encoder::GetButton()
{
    return buttonToggleValue;
}

void Encoder::SetButton(bool button)
{
    buttonToggleValue = button;
}
