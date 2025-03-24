#include "Arduino.h"

#include "AudioTools.h"

// #include "AudioLibs/I2SCodecStream.h"

#include "AudioTools/AudioLibs/AudioBoardStream.h"

// I2SCodecStream i2s(AudioKitEs8388V2); // LyratV43 or LyratV42,AudioKitAC101,AudioKitEs8388V1,AudioKitEs8388V2

AudioInfo info(32000, 2, 16);
SineWaveGenerator<int16_t> sineWave(32000);                // subclass of SoundGenerator with max amplitude of 32000
GeneratedSoundStream<int16_t> sound(sineWave);             // Stream generated from sine wave
AudioBoardStream out(AudioKitEs8388V1);
StreamCopy copier(out, sound);                             // copies sound into i2s

// void setup() {
//   // setup i2s and codec
//   auto cfg = i2s.defaultConfig();
//   cfg.sample_rate = 44100;
//   cfg.bits_per_sample = 16;
//   cfg.channels = 1;
//   i2s.begin();
//   // set volume
//   i2s.setVolume(0.5);
// }

// Arduino Setup
void setup(void) {
  // Open Serial
  Serial.begin(115200);
  AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Info);

  // start I2S
  Serial.println("starting I2S...");
  auto config = out.defaultConfig(TX_MODE);
  config.copyFrom(info);
  out.begin(config);

  // Setup sine wave
  sineWave.begin(info, N_B4);
  Serial.println("started...");
}

// Arduino loop - copy sound to out
void loop() {
  copier.copy();
}