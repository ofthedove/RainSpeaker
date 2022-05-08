
#include "AudioTools.h"
#include "AudioLibs/AudioKit.h"
#include "AudioKitHAL.h"
#include "SPI.h"
#include "SD_MMC.h"

#include "MyAudioPlayer.h"

MySdAudioSource *mySource = new MySdAudioSource();
AudioKitStream kit;
WAVDecoder decoder;
AudioPlayer player(*mySource, kit, decoder);

void setup() {
  Serial.begin(115200);
  // AudioLogger::instance().begin(Serial, AudioLogger::Info);

  // setup output
  auto cfg = kit.defaultConfig(TX_MODE);
  kit.begin(cfg);

  // setup player
  player.setVolume(0.7);
  player.begin();
}

void loop() {
  player.copy();
  kit.processActions();
}
