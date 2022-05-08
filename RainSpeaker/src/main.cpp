
// #define USE_SDFAT

#include "AudioTools.h"
#include "AudioLibs/AudioKit.h"

#include "AudioKitHAL.h"
// #include "SD.h"

#include "SPI.h"
#include "SD_MMC.h"
#include "AudioCodecs/CodecMP3Helix.h"













    class MySdAudioSource : public AudioSource {
    public:
        /// Default constructor
        MySdAudioSource(void) {
          LOGD(LOG_METHOD);

        }

        /// Destructor
        virtual ~MySdAudioSource(){
            LOGD(LOG_METHOD);
            SD_MMC.end();
            Serial.println("Card closed");
        }

        virtual void begin() override {
            LOGD(LOG_METHOD);
            static bool is_sd_setup = false;
            if (!is_sd_setup){
              if (!SD_MMC.begin("/sdcard", true)){
                  Serial.println("Failed to mount SD card");
                  return;
              }

              // Check for an SD card
              sdcard_type_t cardType = SD_MMC.cardType();
              if (cardType == CARD_NONE){
                  Serial.println("No SD card attached");
                  return;
              }

              is_sd_setup = true;
            }
        }

        virtual Stream* nextStream(int offset) override {
            LOGW("-> nextStream: %d", offset);
            file.close();
            file = SD_MMC.open("/RAINLP1.WAV");
            return &file;
        }

        virtual Stream* selectStream(int index) override {
            file.close();
            file = SD_MMC.open("/RAINLP1.WAV");
            return &file;
        }

        virtual Stream* selectStream(const char* path) override {
            file.close();
            file = SD_MMC.open("/RAINLP1.WAV");
            return &file;
        }

    protected:
        File file;
    };

















// AudioKitStream kit;
// AudioKit kit;
// File audioFile;

MySdAudioSource *mySource = new MySdAudioSource();
AudioKitStream kit;
// MP3DecoderHelix decoder;
WAVDecoder decoder;
AudioPlayer player(*mySource, kit, decoder);

void setup() {
  Serial.begin(115200);
  // AudioLogger::instance().begin(Serial, AudioLogger::Info);

  // setup output
  // auto cfg = kit.defaultConfig(TX_MODE);
  // cfg.sd_active = false;
  // cfg.bits_per_sample = 16;
  // kit.begin(cfg);

  // if(!SD.begin(kit.pinSpiCs())){
  //     Serial.println("Card Mount Failed");
  //     return;
  // } else {
  //     Serial.println("Card Mount Success");
  // }

    // switch(cardType)
    // {
    //   case CARD_NONE:
    //     Serial.println("CARD_NONE");
    //     break;
    //   case CARD_MMC:
    //     Serial.println("CARD_MMC");
    //     break;
    //   case CARD_SD:
    //     Serial.println("CARD_SD");
    //     break;
    //   case CARD_SDHC:
    //     Serial.println("CARD_SDHC");
    //     break;
    //   case CARD_UNKNOWN:
    //     Serial.println("CARD_UNKNOWN");
    //     break;
    // }

    // uint64_t cardSize = SD_MMC.cardSize();
    // Serial.printf("cardSize: %d\r\n", cardSize);

    // uint64_t totalBytes = SD_MMC.totalBytes();
    // Serial.printf("totalBytes: %d\r\n", totalBytes);

    // uint64_t usedBytes = SD_MMC.usedBytes();
    // Serial.printf("usedBytes: %d\r\n", usedBytes);



      // setup output
  auto cfg = kit.defaultConfig(TX_MODE);
  kit.begin(cfg);

    // setup player
  player.setVolume(0.7);
  player.begin();
}

void loop() {
  // kit.processActions();
  
  player.copy();
  kit.processActions();
}
