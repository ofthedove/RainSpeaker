
#ifndef Audio_h
#define Audio_h

#include <SPI.h>
#include <SD.h>
#include "AudioTools.h"
#include "AudioTools/AudioLibs/AudioBoardStream.h"
#include "AudioTools/Disk/FileLoop.h"

class Audio {
private:
    const int chipSelect=PIN_AUDIO_KIT_SD_CARD_CS;
    AudioBoardStream i2s(AudioKitEs8388V1); // final output of decoded stream
    WAVDecoder wav;
    EncodedAudioStream decoder(&i2s, &wav); // Decoding stream
    FileLoop loopingFile;
    StreamCopy copier(decoder, loopingFile, 4096);
public:
   Audio();

   ~Audio();

   void loop();
   void setVolume(int volume)
};

#endif
