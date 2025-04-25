#include "Audio.h"

Audio::Audio(Print& logPrint)
{
    i2s = new AudioBoardStream(AudioKitEs8388V1); // final output of decoded stream
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
    SD.begin(chipSelect, SPI, 10000000);
    loopingFile.setFile(SD.open("/RAINLP1.WAV"));
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