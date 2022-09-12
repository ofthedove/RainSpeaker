
#include "AudioTools.h"
#include "AudioLibs/AudioKit.h"
#include "AudioKitHAL.h"
#include "SPI.h"
#include "SD_MMC.h"
#include "HeartbeatLed.h"

#include "MyAudioPlayer.h"

enum
{
  taskStackSize = 10000,
  InitialVolume = 70,
  VolumeFadeRate = 8,
  VolumeChangeMultiplier = 2,

  VolumeMin = 0,
  VolumeMax = 100,
};

MySdAudioSource *mySource = new MySdAudioSource();
AudioKitStream kit;
WAVDecoder decoder;
AudioPlayer player(*mySource, kit, decoder);

#include "Bluetooth.h"

Bluetooth bt = Bluetooth();
uint8_t volume;
uint8_t prevVol;
int targetVolume = InitialVolume;
int actualVolume = InitialVolume;

HeartbeatLed hbLed = HeartbeatLed(selfAudioKit->pinGreenLed());

TaskHandle_t BleTaskHandle;
TaskHandle_t VolumeTaskHandle;
TaskHandle_t AudioTaskHandle;

void BluetoothTask(void *parameter)
{
  (void)parameter;

  while (true)
  {
    delay(500);

    bt.Run();
  }
}

static void Clamp(int min, int *value, int max)
{
  if (*value < min)
  {
    *value = min;
  }
  else if (*value > max)
  {
    *value = max;
  }
}

void VolumeTask(void *parameter)
{
  (void)parameter;

  while (true)
  {
    delay(50);

    int delta = volume - prevVol;
    if (delta != 0)
    {
      prevVol = volume;

      if (delta > 128)
      {
        delta = delta - 256;
      }

      if (delta < -128)
      {
        delta = delta + 256;
      }

      delta *= VolumeChangeMultiplier;

      targetVolume += delta;

      Clamp(VolumeMin, &targetVolume, VolumeMax);

      if (targetVolume > actualVolume)
      {
        actualVolume += VolumeFadeRate;
        Clamp(VolumeMin, &actualVolume, targetVolume);
      }
      else if (targetVolume < actualVolume)
      {
        actualVolume -= VolumeFadeRate;
        Clamp(targetVolume, &actualVolume, VolumeMax);
      }

      float vol = (float)actualVolume / 100.0;
      player.setVolume(vol);
    }
  }
}

void AudioTask(void *parameter)
{
  (void)parameter;

  while (true)
  {
    delay(1);

    player.copy();
    kit.processActions();
  }
}

void setup()
{
  Serial.begin(115200);
  // AudioLogger::instance().begin(Serial, AudioLogger::Info);

  bt.Init(&hbLed);
  bt.SetVolumePointer(&volume);

  // setup output
  auto cfg = kit.defaultConfig(TX_MODE);
  kit.begin(cfg);
  kit.setVolume(100);

  mySource->setTimeoutAutoNext(10);

  // setup player
  player.setVolume(0.7);
  player.begin();

  xTaskCreate(BluetoothTask, "BLE Task", taskStackSize, NULL, 3, &BleTaskHandle);
  xTaskCreate(VolumeTask, "Volume Task", taskStackSize, NULL, 2, &VolumeTaskHandle);
  xTaskCreate(AudioTask, "Audio Task", taskStackSize, NULL, 5, &AudioTaskHandle);
}

void loop()
{
  delay(1000);
}
