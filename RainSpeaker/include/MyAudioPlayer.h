#ifndef MYAUDIOPLAYER_H
#define MYAUDIOPLAYER_H

#include "AudioTools.h"
#include "SD_MMC.h"

class MySdAudioSource : public AudioSource
{
public:
    /// Default constructor
    MySdAudioSource(void)
    {
        LOGD(LOG_METHOD);
    }

    /// Destructor
    virtual ~MySdAudioSource()
    {
        LOGD(LOG_METHOD);
        SD_MMC.end();
        Serial.println("Card closed");
    }

    virtual void begin() override
    {
        LOGD(LOG_METHOD);
        static bool is_sd_setup = false;
        if (!is_sd_setup)
        {
            if (!SD_MMC.begin("/sdcard", true))
            {
                Serial.println("Failed to mount SD card");
                return;
            }

            // Check for an SD card
            sdcard_type_t cardType = SD_MMC.cardType();
            if (cardType == CARD_NONE)
            {
                Serial.println("No SD card attached");
                return;
            }

            is_sd_setup = true;
        }
    }

    virtual Stream *nextStream(int offset) override
    {
        LOGW("-> nextStream: %d", offset);
        file.close();
        file = SD_MMC.open("/RAINLP1.WAV");
        return &file;
    }

    virtual Stream *selectStream(int index) override
    {
        file.close();
        file = SD_MMC.open("/RAINLP1.WAV");
        return &file;
    }

    virtual Stream *selectStream(const char *path) override
    {
        file.close();
        file = SD_MMC.open("/RAINLP1.WAV");
        return &file;
    }

protected:
    File file;
};

#endif