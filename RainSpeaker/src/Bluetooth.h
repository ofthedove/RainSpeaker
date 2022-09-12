#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "BLEDevice.h"
#include "HeartbeatLed.h"

using std::uint8_t;

class Bluetooth
{
public:
    Bluetooth();

    void Init(HeartbeatLed *statusLed);
    void Run();
    void SetVolumeCallback(void (*callback)(uint8_t vol));
    void SetPausePointer(bool *pause);

private:
};

#endif
