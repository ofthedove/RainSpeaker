
#include "AudioTools.h"
#include "AudioLibs/AudioKit.h"
#include "AudioKitHAL.h"
#include "Bluetooth.h"
#include "HeartbeatLed.h"
#include "MyAudioPlayer.h"
#include "SPI.h"
#include "SD_MMC.h"
#include "VolumeManager.h"

using std::uint8_t;

enum
{
  taskStackSize = 10000,
};

MySdAudioSource *mySource = new MySdAudioSource();
AudioKitStream kit;
WAVDecoder decoder;
AudioPlayer player(*mySource, kit, decoder);

// Bluetooth bt = Bluetooth();
HeartbeatLed hbLed = HeartbeatLed(selfAudioKit->pinGreenLed());

TaskHandle_t BleTaskHandle;
TaskHandle_t AudioTaskHandle;

// This kinda sucks but passing a pointer to player requires a second include
//    of `AudioTools.h` which breaks linking. I blame the library.
static void SetPlayerVolume(float volume)
{
  player.setVolume(volume);
}

VolumeManager volumeManager = VolumeManager(SetPlayerVolume);

// static void BluetoothVolumeCallback(uint8_t rawVolume)
// {
//   volumeManager.UpdateRawVolumeInput(rawVolume);
// }

// static void BluetoothTask(void *parameter)
// {
//   (void)parameter;

//   while (true)
//   {
//     delay(500);

//     bt.Run();
//   }
// }

static void AudioTask(void *parameter)
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

  // bt.Init(&hbLed);
  // bt.SetVolumeCallback(BluetoothVolumeCallback);

  // setup output
  auto cfg = kit.defaultConfig(TX_MODE);
  kit.begin(cfg);
  kit.setVolume(100);

  mySource->setTimeoutAutoNext(10);

  // setup player
  player.setVolume(0.7);
  player.begin();

  // xTaskCreate(BluetoothTask, "BLE Task", taskStackSize, NULL, 3, &BleTaskHandle);
  xTaskCreate(AudioTask, "Audio Task", taskStackSize, NULL, 5, &AudioTaskHandle);
}

void loop()
{
  delay(1000);
}
