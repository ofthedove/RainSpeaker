#ifndef AudioWithRamp_h
#define AudioWithRamp_h

#include "Audio.h"

class AudioWithRamp : public Audio {
public:
    AudioWithRamp(Print& logPrint, int initialVolume = 50, int rampTimeMs = 2000);
    void setRampTime(int timeMs);
    void setVolume(int volume);
    void loop();

private:
    int rampTimeMs;
    int targetVolume;

    int startTimeMs;
    int startVolume;
};

#endif