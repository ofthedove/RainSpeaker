
#include "AudioTools.h"
#include "AudioLibs/AudioKit.h"

AudioKitStream kit;

void setup() {
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Info);

  // setup output
  auto cfg = kit.defaultConfig(TX_MODE);
  cfg.sd_active = false;
  cfg.bits_per_sample = 16;
  kit.begin(cfg);
}

void loop() {
  kit.processActions();
}
