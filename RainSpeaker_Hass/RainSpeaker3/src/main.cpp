// #include "Arduino.h"

// #include "AudioTools.h"

// // #include "AudioLibs/I2SCodecStream.h"

// #include "AudioTools/AudioLibs/AudioBoardStream.h"

// // I2SCodecStream i2s(AudioKitEs8388V2); // LyratV43 or LyratV42,AudioKitAC101,AudioKitEs8388V1,AudioKitEs8388V2

// AudioInfo info(32000, 2, 16);
// SineWaveGenerator<int16_t> sineWave(32000);                // subclass of SoundGenerator with max amplitude of 32000
// GeneratedSoundStream<int16_t> sound(sineWave);             // Stream generated from sine wave
// AudioBoardStream out(AudioKitEs8388V1);
// StreamCopy copier(out, sound);                             // copies sound into i2s

// // void setup() {
// //   // setup i2s and codec
// //   auto cfg = i2s.defaultConfig();
// //   cfg.sample_rate = 44100;
// //   cfg.bits_per_sample = 16;
// //   cfg.channels = 1;
// //   i2s.begin();
// //   // set volume
// //   i2s.setVolume(0.5);
// // }

// // Arduino Setup
// void setup(void) {
//   // Open Serial
//   Serial.begin(115200);
//   AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Info);

//   // start I2S
//   Serial.println("starting I2S...");
//   auto config = out.defaultConfig(TX_MODE);
//   config.copyFrom(info);
//   out.begin(config);

//   // Setup sine wave
//   sineWave.begin(info, N_B4);
//   Serial.println("started...");
// }

// // Arduino loop - copy sound to out
// void loop() {
//   copier.copy();
// }


/**
 * @file streams-file_loop-audiokit.ino
 * @author Phil Schatzmann
 * @brief Just a small demo, how to use looping (=repeating) files with the SD library
 * @version 0.1
 * @date 2022-10-09
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <SPI.h>
#include <SD.h>
#include "AudioTools.h"
#include "AudioTools/AudioLibs/AudioBoardStream.h"
#include "AudioTools/Disk/FileLoop.h"
// #include "AudioTools/AudioCodecs/CodecMP3Helix.h"


const int chipSelect=PIN_AUDIO_KIT_SD_CARD_CS;
AudioBoardStream i2s(AudioKitEs8388V1); // final output of decoded stream
// EncodedAudioStream decoder(&i2s, new MP3DecoderHelix()); // Decoding stream
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
  loopingFile.setLoopCount(2); // define loop count
  //audioFile.setStartPos(44); // restart from pos 44
  //if ((audioFile.size()-44) % 1024!=0) audioFile.setSize((((audioFile.size()-44)/1024)+1)*1024+44);
  loopingFile.begin();

  // setup I2S based on sampling rate provided by decoder
  decoder.begin();
}

void loop(){
  copier.copy();
}




















// /**
//  * @file streams-audiokit-sd-audiokit.ino
//  * @author Phil Schatzmann
//  * @brief We record the input from the microphone to a file and constantly repeat to play the file
//  * The input is triggered by pressing key 1. Recording stops when key 1 is released!
//  * @version 0.1
//  * @date 2022-09-01
//  *
//  * @copyright Copyright (c) 2022
//  *
//  */
// #include "AudioTools.h"
// #include "AudioTools/AudioLibs/AudioBoardStream.h"
// #include <SPI.h>
// #include <SD.h>

// const char *file_name = "/rec.raw";
// AudioInfo info(16000, 1, 16);
// AudioBoardStream kit(AudioKitEs8388V1);
// File file;   // final output stream
// StreamCopy copier; // copies data
// bool recording = false;  // flag to make sure that close is only executed one
// uint64_t end_time; // trigger to call endRecord


// void record_start(bool pinStatus, int pin, void* ref){
//   Serial.println("Recording...");
//   // open the output file
//   file = SD.open(file_name, FILE_WRITE);
//   copier.begin(file, kit);
//   recording = true;
// }

// void record_end(bool pinStatus, int pin, void* ref){
//   if (recording == true){
//     Serial.println("Playing...");
//     file.close();
//     recording = false;
//     file = SD.open(file_name); // reopen in read mode
//     copier.begin(kit, file);  // start playback
//   }
// }

// void setup(){
//   Serial.begin(115200);
//   while(!Serial); // wait for serial to be ready
//   AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Warning);

//   // setup input and output: setup audiokit before SD!
//   auto cfg = kit.defaultConfig(RXTX_MODE);
//   cfg.sd_active = true;
//   cfg.copyFrom(info);
//   cfg.input_device = ADC_INPUT_LINE2;
//   kit.begin(cfg);
  // kit.setVolume(1.0);

//   // Open SD drive
//   if (!SD.begin(PIN_AUDIO_KIT_SD_CARD_CS)) {
//     Serial.println("Initialization failed!");
//     while (1); // stop
//   }
//   Serial.println("Initialization done.");


//   // record when key 1 is pressed
//   kit.audioActions().add(kit.getKey(1), record_start, record_end);
//   Serial.println("Press Key 1 to record");

// }

// void loop(){

//   // record or play file
//   copier.copy();

//   // while playing: at end of file -> reposition to beginning
//   if (!recording && file.size()>0 && file.available()==0){
//       file.seek(0);
//       Serial.println("Replay...");
//   }

//   // Process keys
//   kit.processActions();

// }