#ifndef VOLUMEMANAGER_H
#define VOLUMEMANAGER_H

#include <Arduino.h>
#include <stdint.h>

enum
{
    InitialVolume = 70,
};

class VolumeManager
{
public:
    VolumeManager(void (*setPlayerVolume)(float));

    void UpdateRawVolumeInput(uint8_t volume);

    // private: // Can't actually be private, needed by task
    void (*setPlayerVolume)(float);
    uint8_t volume;
    uint8_t prevVol;
    int targetVolume = InitialVolume;
    int actualVolume = InitialVolume;

private:
    TaskHandle_t VolumeManagerTaskHandle;
};

#endif
