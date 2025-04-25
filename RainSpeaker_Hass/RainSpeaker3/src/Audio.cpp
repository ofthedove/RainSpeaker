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
    i2s = new AudioBoardStream(LyratV43);
#else /* BOARD_AUDIOKIT_ES8388V1 */
    i2s = new AudioBoardStream(AudioKitEs8388V1); // final output of decoded stream
#endif
    decoder = new EncodedAudioStream(i2s, &wav); // Decoding stream
    copier = new StreamCopy(*decoder, loopingFile, 4096);

    // Must be Warning or above! Info and Debug cause stuttering audio output
    AudioToolsLogger.begin(logPrint, AudioToolsLogLevel::Warning);

    // setup audiokit before SD!
    auto config = i2s->defaultConfig(TX_MODE);
    config.sd_active = true;
    i2s->begin(config);
    i2s->setVolume(0.5);

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

    if(i2s->hasBoard())
    {
      AudioBoard &board = i2s->board();
      board.setVolume(volume);
    }
}