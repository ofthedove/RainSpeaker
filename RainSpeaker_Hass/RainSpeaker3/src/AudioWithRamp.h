#ifndef AudioWithRamp_h
#define AudioWithRamp_h

#include "Audio.h"

class AudioWithRamp : public Audio {
public:
    AudioWithRamp(Print& logPrint, int initialVolume = 50, int rampTimeMs = 2000);

    // Ramp time is the time it takes to ramp from 0 to full volume in ms
    void setRampTime(int timeMs);
    int getRampTime();

    // Set the target volume that will be ramped to
    void setVolume(int volume);

    void loop();

private:
    int rampTimeMs;
    int targetVolume;

    int startTimeMs;
    int startVolume;
    int currentRampDurationMs;
};

#endif