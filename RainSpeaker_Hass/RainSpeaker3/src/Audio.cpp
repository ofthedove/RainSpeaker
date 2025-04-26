#include "Audio.h"

#ifdef BOARD_LYRAT
#include "SD_MMC.h"
#else /* BOARD_AUDIOKIT_ES8388V1 */
#include <SPI.h>
#include <SD.h>
#endif

Audio::Audio(Print& logPrint)
{
#ifdef BOARD_LYRAT
    kit = new AudioBoardStream(LyratV43);
#else /* BOARD_AUDIOKIT_ES8388V1 */
    kit = new AudioBoardStream(AudioKitEs8388V1); // final output of decoded stream
#endif
    // decoder = new EncodedAudioStream(kit, &decoder); // Decoding stream
    decoder = new WAVDecoder();
    // copier = new StreamCopy(*decoder, loopingFile, 4096);
    source = new AudioSourceSDMMC("/", "WAV");
    player = new AudioPlayer(*source, *kit, *decoder);

    // Must be Warning or above! Info and Debug cause stuttering audio output
    AudioToolsLogger.begin(logPrint, AudioToolsLogLevel::Info);

    // setup audiokit before SD!
    auto config = kit->defaultConfig(TX_MODE);
    config.sd_active = true;
    kit->begin(config);
    // kit->setVolume(0.5);

// #ifdef BOARD_LYRAT
//     // Need to turn amp on and off see https://www.pschatzmann.ch/home/2024/11/03/the-lyrat-mini-board-is-supported-now-as-well/
//     kit->addHeadphoneDetectionAction();
// #else /* BOARD_AUDIOKIT_ES8388V1 */
// #endif


//     // setup file
// #ifdef BOARD_LYRAT
//     SD_MMC.begin("/sdcard", true);
//     loopingFile.setFile(SD_MMC.open("/RAINLP1.WAV"));
// #else /* BOARD_AUDIOKIT_ES8388V1 */
//     SD.begin(chipSelect, SPI, 10000000);
//     loopingFile.setFile(SD.open("/RAINLP1.WAV"));
// #endif
//     loopingFile.begin();

    // setup I2S based on sampling rate provided by decoder
    // decoder->begin();

    kit->setVolume(1.0);
    player->setVolume(1.0);
    player->begin();
}

Audio::~Audio() {
    // do nothing?
}

void Audio::loop() {
    // copier->copy();
    player->copy();
    kit->processActions();
}

void Audio::setVolume(int volume) {
    // if(volume < 0)
    // {
    //     volume = 0;
    // }
    // else if(volume > 100)
    // {
    //     volume = 100;
    // }

    // // kit->setVolume((float)volume / 100.0);
    // player->setVolume((float)volume / 100.0);

    // if(kit->hasBoard())
    // {
    //   AudioBoard &board = kit->board();
    //   board.setVolume(volume);
    // }
}