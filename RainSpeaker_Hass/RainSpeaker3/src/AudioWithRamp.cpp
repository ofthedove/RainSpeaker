#include "AudioWithRamp.h"
#include <Arduino.h>

enum
{
    MinVolume = 0,
    MaxVolume = 100,
    MinDuration = 0,
};

AudioWithRamp::AudioWithRamp(Print& logPrint, int initialVolume, int rampTimeMs) : Audio(logPrint) {
    this->rampTimeMs = rampTimeMs;
    Audio::setVolume(0); // Ramp from 0 to initialVolume
    setVolume(initialVolume);
}

void AudioWithRamp::setRampTime(int timeMs) {
    rampTimeMs = timeMs;
}

int AudioWithRamp::getRampTime() {
    return rampTimeMs;
}

void AudioWithRamp::setVolume(int volume) {
    if(volume < MinVolume)
    {
        volume = MinVolume;
    }
    else if(volume > MaxVolume)
    {
        volume = MaxVolume;
    }

    targetVolume = volume;
    startVolume = getVolume();
    startTimeMs = millis();

    int deltaVolume = abs(targetVolume - startVolume);
    currentRampDurationMs = map(deltaVolume, MinVolume, MaxVolume, MinDuration, rampTimeMs);
}

void AudioWithRamp::loop() {
    int elapsed = millis() - startTimeMs;\

    if (elapsed >= currentRampDurationMs) {
        Audio::setVolume(targetVolume);
    } else {
        int currentVolume = map(elapsed, 0, currentRampDurationMs, startVolume, targetVolume);
        Audio::setVolume(currentVolume);
    }

    Audio::loop();
}