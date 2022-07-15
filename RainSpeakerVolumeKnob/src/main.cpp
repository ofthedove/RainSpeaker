#include <Arduino.h>
#include "Bluetooth.h"
#include "Encoder.h"

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

using std::uint8_t;

enum
{
  ledBuiltinPin = LED_BUILTIN,
};

Encoder &encoder = Encoder::getInstance();
Bluetooth bluetooth = Bluetooth();

void setup()
{
  Serial.begin(115200);

  pinMode(ledBuiltinPin, OUTPUT);

  encoder.Init();
  bluetooth.Init();
}

void loop()
{
  encoder.Run();

  static bool prevButton = false;
  static uint8_t prevPosition = 0;

  bool button = encoder.GetButton();
  uint8_t position = encoder.GetPosition();

  if (prevButton != button || prevPosition != position)
  {
    Serial.print(button);
    Serial.print("  ");
    Serial.println(position);

    bluetooth.SetButton(button);
    bluetooth.SetPosition(position);

    digitalWrite(ledBuiltinPin, button);

    prevButton = button;
    prevPosition = position;
  }

  delay(50);
}