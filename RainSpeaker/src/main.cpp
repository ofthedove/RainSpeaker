
#include "AudioTools.h"
#include "AudioLibs/AudioKit.h"
#include "AudioKitHAL.h"
#include "SPI.h"
#include "SD_MMC.h"

#include "MyAudioPlayer.h"

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "97b352c8-651f-4e8c-aeb2-38a42cef3637"
#define CHARACTERISTIC_UUID "e184f3a9-8b91-4a0a-b4c1-79358a43075f"

BLECharacteristic *pCharacteristic;
uint16_t prevVol = 70;

MySdAudioSource *mySource = new MySdAudioSource();
AudioKitStream kit;
WAVDecoder decoder;
AudioPlayer player(*mySource, kit, decoder);

void setup() {
  Serial.begin(115200);
  // AudioLogger::instance().begin(Serial, AudioLogger::Info);

  BLEDevice::init("Rain Speaker");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setValue(prevVol);
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");


  // setup output
  auto cfg = kit.defaultConfig(TX_MODE);
  kit.begin(cfg);
  kit.setVolume(prevVol);

  // setup player
  player.setVolume(1.0);
  player.begin();
}

void loop() {
  player.copy();
  kit.processActions();

  uint16_t vol = *(uint16_t *)pCharacteristic->getData();
  if(vol != prevVol)
  {
    kit.setVolume(vol);
    Serial.printf("Volume set to: %d", vol);
    prevVol = vol;
  }
}
