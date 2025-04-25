
#ifndef Audio_h
#define Audio_h

#include "AudioTools.h"
#include "AudioTools/AudioLibs/AudioBoardStream.h"
#include "AudioTools/Disk/FileLoop.h"

class Audio {
private:
    const int chipSelect=PIN_AUDIO_KIT_SD_CARD_CS;
    AudioBoardStream *i2s; // final output of decoded stream
    WAVDecoder wav;
    EncodedAudioStream *decoder; // Decoding stream
    FileLoop loopingFile;
    StreamCopy *copier;
public:
   Audio(Print& logPrint);

   ~Audio();

   void loop();
   void setVolume(int volume);
};

#endif
