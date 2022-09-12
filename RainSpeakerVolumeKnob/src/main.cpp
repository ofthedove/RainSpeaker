#include <Arduino.h>
#include "Bluetooth.h"
#include "Encoder.h"
#include "HeartbeatLed.h"

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

using std::uint8_t;

enum
{
  ledBuiltinPin = LED_BUILTIN,
  taskStackSize = 10000,
};

Encoder &encoder = Encoder::getInstance();
Bluetooth bluetooth = Bluetooth();
HeartbeatLed hbLed = HeartbeatLed(ledBuiltinPin);

TaskHandle_t BleTaskHandle;

void BluetoothTask(void *parameter)
{
  (void)parameter;

  while (true)
  {
    delay(200);

    bool button = encoder.GetButton();
    bluetooth.SetButton(button);

    uint8_t position = encoder.GetPosition();
    bluetooth.SetPosition(position);
  }
}

void setup()
{
  Serial.begin(115200);

  pinMode(ledBuiltinPin, OUTPUT);

  encoder.Init();
  bluetooth.Init(&hbLed);

  xTaskCreate(BluetoothTask, "BLE Task", taskStackSize, NULL, 1, &BleTaskHandle);
}

void loop()
{
  encoder.Run();
  delay(50);
}