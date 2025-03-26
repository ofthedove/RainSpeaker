
#include <SPI.h>
#include <SD.h>
#include "AudioTools.h"
#include "AudioTools/AudioLibs/AudioBoardStream.h"
#include "AudioTools/Disk/FileLoop.h"


const int chipSelect=PIN_AUDIO_KIT_SD_CARD_CS;
AudioBoardStream i2s(AudioKitEs8388V1); // final output of decoded stream
WAVDecoder wav;
EncodedAudioStream decoder(&i2s, &wav); // Decoding stream
FileLoop loopingFile;
StreamCopy copier(decoder, loopingFile, 4096);

void setup(){
  Serial.begin(115200);
  AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Warning);

  // setup audiokit before SD!
  auto config = i2s.defaultConfig(TX_MODE);
  config.sd_active = true;
  i2s.begin(config);
  i2s.setVolume(1.0);

  // setup file
  SD.begin(chipSelect, SPI, 10000000);
  loopingFile.setFile(SD.open("/RAINLP1.WAV"));
  loopingFile.begin();

  // setup I2S based on sampling rate provided by decoder
  decoder.begin();
}

void loop(){
  copier.copy();
}
