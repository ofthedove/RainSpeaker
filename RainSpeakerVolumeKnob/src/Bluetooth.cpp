#include <Arduino.h>
#include "Bluetooth.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID "ff149c8e-59bf-497d-a11f-d5af3128a195"
#define CHARACTERISTIC_UUID_ENC "be516360-2361-45e2-aa33-a157d48d7bc7"
#define CHARACTERISTIC_UUID_BTN "6ba8bfa1-68f0-44dd-bdff-434016586344"

using std::uint8_t;

Bluetooth::Bluetooth()
{
}

void Bluetooth::Init()
{
    Serial.println("Starting BLE");

    BLEDevice::init("Rain Knob");
    BLEServer *pServer = BLEDevice::createServer();
    BLEService *pService = pServer->createService(SERVICE_UUID);

    uint8_t zero = 0;

    pCharacteristicEnc = pService->createCharacteristic(
        CHARACTERISTIC_UUID_ENC,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_WRITE);
    pCharacteristicEnc->setValue(&zero, sizeof(zero));

    pCharacteristicBtn = pService->createCharacteristic(
        CHARACTERISTIC_UUID_BTN,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_WRITE);
    pCharacteristicBtn->setValue(&zero, sizeof(zero));

    pService->start();
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();

    Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void Bluetooth::SetPosition(uint8_t position)
{
    static uint8_t prevPos = 0;
    if (position != prevPos)
    {
        pCharacteristicEnc->setValue(&position, sizeof(position));
        pCharacteristicEnc->notify();
    }
    prevPos = position;
}

void Bluetooth::SetButton(uint8_t button)
{
    static uint8_t prevBtn = 0;
    if (button != prevBtn)
    {
        pCharacteristicBtn->setValue(&button, sizeof(button));
        pCharacteristicBtn->notify();
    }
    prevBtn = button;
}
