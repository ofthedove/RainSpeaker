
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

#include "Bluetooth.h"

Bluetooth bt = Bluetooth();
uint8_t volume;
uint8_t prevVol;
int actualVolume = 70;

void setup()
{
  Serial.begin(115200);
  // AudioLogger::instance().begin(Serial, AudioLogger::Info);

  bt.Init();
  bt.SetVolumePointer(&volume);

  // setup output
  auto cfg = kit.defaultConfig(TX_MODE);
  kit.begin(cfg);
  kit.setVolume(100);

  mySource->setTimeoutAutoNext(10);

  // setup player
  player.setVolume(0.7);
  player.begin();
}

void loop()
{
  static unsigned long nextMilli = millis() + 250;

  // Need to handle overflow
  if (millis() > nextMilli)
  {
    nextMilli += 250;
    bt.Run();

    int delta = volume - prevVol;
    Serial.print(volume);
    Serial.print(" | ");
    Serial.print(prevVol);
    Serial.print(" | ");
    Serial.print(delta);
    Serial.print(" | ");

    prevVol = volume;

    if (delta > 128)
    {
      delta = delta - 256;
    }

    if (delta < -128)
    {
      delta = delta + 256;
    }

    Serial.print(delta);
    Serial.print(" | ");

    actualVolume += delta;

    Serial.print(actualVolume);
    Serial.print(" | ");

    if (actualVolume < 0)
    {
      actualVolume = 0;
    }

    if (actualVolume > 100)
    {
      actualVolume = 100;
    }

    Serial.print(actualVolume);
    Serial.print(" | ");

    float vol = (float)actualVolume / 100.0;
    Serial.println(vol);
    player.setVolume(vol);
  }

  player.copy();
  kit.processActions();
}
