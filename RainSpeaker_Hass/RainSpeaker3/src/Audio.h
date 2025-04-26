
#ifndef Audio_h
#define Audio_h

#include "AudioTools.h"
#include "AudioTools/AudioLibs/AudioBoardStream.h"
// #include "AudioTools/Disk/FileLoop.h"
#include "AudioTools/Disk/AudioSourceSDMMC.h"
#include "AudioTools/AudioCodecs/CodecWAV.h"

class Audio {
private:
    const int chipSelect=PIN_AUDIO_KIT_SD_CARD_CS;
    AudioBoardStream *kit; // final output of decoded stream
    WAVDecoder *decoder;
    // EncodedAudioStream *decoder; // Decoding stream
    // FileLoop loopingFile;
    // StreamCopy *copier;
    AudioSourceSDMMC *source;
    AudioPlayer *player;

public:
   Audio(Print& logPrint);

   ~Audio();

   void loop();
   void setVolume(int volume);
};

#endif
