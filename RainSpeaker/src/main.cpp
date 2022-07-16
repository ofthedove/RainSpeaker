
#include "AudioTools.h"
#include "AudioLibs/AudioKit.h"
#include "AudioKitHAL.h"
#include "SPI.h"
#include "SD_MMC.h"

#include "MyAudioPlayer.h"

#include "Bluetooth.h"

uint16_t prevVol = 70;

MySdAudioSource *mySource = new MySdAudioSource();
AudioKitStream kit;
WAVDecoder decoder;
AudioPlayer player(*mySource, kit, decoder);

Bluetooth bt = Bluetooth();

uint8_t volume;

void setup()
{
  Serial.begin(115200);
  // AudioLogger::instance().begin(Serial, AudioLogger::Info);

  // setup output
  auto cfg = kit.defaultConfig(TX_MODE);
  kit.begin(cfg);
  kit.setVolume(prevVol);

  // setup player
  player.setVolume(1.0);
  player.begin();

  bt.Init();
  bt.SetVolumePointer(&volume);
}

void loop()
{
  // player.copy();
  // kit.processActions();
  bt.Run();
  delay(500);
  Serial.println(volume);
}
