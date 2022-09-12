#include "VolumeManager.h"

enum
{
    taskStackSize = 10000,
    taskPriority = 2,

    VolumeFadeRate = 8,
    VolumeChangeMultiplier = 2,

    VolumeMin = 0,
    VolumeMax = 100,
};

static void Clamp(volatile int min, volatile int *value, int max)
{
    if (*value < min)
    {
        *value = min;
    }
    else if (*value > max)
    {
        *value = max;
    }
}

void VolumeManagerTask(void *parameter)
{
    volatile VolumeManager *instance = (VolumeManager *)parameter;

    while (true)
    {
        delay(50);

        int delta = instance->volume - instance->prevVol;
        if (delta != 0)
        {
            instance->prevVol = instance->volume;

            if (delta > 128)
            {
                delta = delta - 256;
            }

            if (delta < -128)
            {
                delta = delta + 256;
            }

            delta *= VolumeChangeMultiplier;

            instance->targetVolume += delta;

            Clamp(VolumeMin, &instance->targetVolume, VolumeMax);

            if (instance->targetVolume > instance->actualVolume)
            {
                instance->actualVolume += VolumeFadeRate;
                Clamp(VolumeMin, &instance->actualVolume, instance->targetVolume);
            }
            else if (instance->targetVolume < instance->actualVolume)
            {
                instance->actualVolume -= VolumeFadeRate;
                Clamp(instance->targetVolume, &instance->actualVolume, VolumeMax);
            }

            float vol = (float)instance->actualVolume / 100.0;
            instance->setPlayerVolume(vol);
        }
    }
}

VolumeManager::VolumeManager(void (*setPlayerVolume)(float))
{
    this->setPlayerVolume = setPlayerVolume;

    xTaskCreate(VolumeManagerTask, "Volume Manager Task", taskStackSize, this, taskPriority, &VolumeManagerTaskHandle);
}

void VolumeManager::UpdateRawVolumeInput(uint8_t volume)
{
    this->volume = volume;
}