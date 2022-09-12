#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <cstdint>
#include <BLECharacteristic.h>
#include "HeartbeatLed.h"

using std::uint8_t;

class Bluetooth
{
public:
    Bluetooth();

    void Init(HeartbeatLed *statusLed);
    void SetPosition(uint8_t position);
    void SetButton(uint8_t button);

private:
    BLECharacteristic *pCharacteristicEnc;
    BLECharacteristic *pCharacteristicBtn;
};

#endif
