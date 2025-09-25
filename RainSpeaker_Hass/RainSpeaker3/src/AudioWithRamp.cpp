#include "AudioWithRamp.h"
#include <Arduino.h>

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
    if(volume < 0)
    {
        volume = 0;
    }
    else if(volume > 100)
    {
        volume = 100;
    }

    targetVolume = volume;
    startVolume = getVolume();
    startTimeMs = millis();
}

void AudioWithRamp::loop() {
    int elapsed = millis() - startTimeMs;
    if (elapsed >= rampTimeMs) {
        Audio::setVolume(targetVolume);
    } else {
        float progress = (float)elapsed / rampTimeMs;
        int currentVolume = startVolume + (int)((targetVolume - startVolume) * progress);
        Audio::setVolume(currentVolume);
    }

    Audio::loop();
}