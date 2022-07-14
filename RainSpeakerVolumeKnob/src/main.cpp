#include <Arduino.h>
#include "Encoder.h"

enum
{
  ledBuiltinPin = LED_BUILTIN,
};

Encoder &encoder = Encoder::getInstance();

void setup()
{
  pinMode(ledBuiltinPin, OUTPUT);

  encoder.Init();

  Serial.begin(115200);
}

void loop()
{
  encoder.Run();

  Serial.print(encoder.GetButton());
  Serial.print("  ");
  Serial.println(encoder.GetPosition());

  digitalWrite(ledBuiltinPin, encoder.GetButton());

  delay(50);
}