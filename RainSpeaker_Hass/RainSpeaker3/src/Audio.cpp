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
    decoder = new EncodedAudioStream(kit, &wav); // Decoding stream
    copier = new StreamCopy(*decoder, loopingFile, 4096);

    // Must be Warning or above! Info and Debug cause stuttering audio output
    // For debugging, set to Info (or Debug)
    // At info, you can add -DCOPY_LOG_OFF to get better debug with little/no stutter
    // To get driver level debug, you must modify ConfigAudioDriver.h to define debug level as 0
    AudioToolsLogger.begin(logPrint, AudioToolsLogLevel::Warning);

    // setup audiokit before SD!
    auto config = kit->defaultConfig(TX_MODE);
    config.sd_active = true;
    kit->begin(config);
    kit->setVolume(0.5);

#ifdef BOARD_LYRAT
    // Need to turn amp on and off see https://www.pschatzmann.ch/home/2024/11/03/the-lyrat-mini-board-is-supported-now-as-well/
    kit->addHeadphoneDetectionAction();
#else /* BOARD_AUDIOKIT_ES8388V1 */
#endif


    // setup file
#ifdef BOARD_LYRAT
    SD_MMC.begin("/sdcard", true);
    loopingFile.setFile(SD_MMC.open("/RAINLP1.WAV"));
#else /* BOARD_AUDIOKIT_ES8388V1 */
    SD.begin(chipSelect, SPI, 10000000);
    loopingFile.setFile(SD.open("/RAINLP1.WAV"));
#endif
    loopingFile.begin();

    // setup I2S based on sampling rate provided by decoder
    decoder->begin();
}

Audio::~Audio() {
    // do nothing?
}

void Audio::loop() {
    copier->copy();
    kit->processActions();
}

void Audio::setVolume(int volume) {
    if(volume < 0)
    {
        volume = 0;
    }
    else if(volume > 100)
    {
        volume = 100;
    }

    // kit->setVolume((float)volume / 100.0);
    // Instead of doing a float division just for kit to multiply it back,
    //   directly access the board pointer and set the integer volume
    if(kit->hasBoard())
    {
      AudioBoard &board = kit->board();
      board.setVolume(volume);
    }
}