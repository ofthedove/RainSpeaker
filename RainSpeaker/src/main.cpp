/* Kit-bashed from examples
 * basic-a2dp-audiokit
 * ~streams-a2dp-serial~ That didn't work...
 */

#include "AudioTools.h"
#include "AudioLibs/AudioKit.h"
#include "AudioLibs/AudioA2DP.h"
#include "knghtsng.h"

BluetoothA2DPSink a2dp_sink;
AudioKitStream kit;

MemoryStream wav(knghtsng_wav, knghtsng_wav_len);
MemoryStream *noiseStream = new MemoryStream();
AdapterAudioStreamToAudioPrint noisePrint(*noiseStream);
EncodedAudioStream enc(&noisePrint, new WAVDecoder());
StreamCopy copier(enc, wav);    // copy in to out

void read_data_stream(const uint8_t *data, uint32_t length) {
  for(uint32_t i = length; i > 0; i--)
  {
    uint8_t sample = (uint8_t)noiseStream->read();
    kit.write(&sample, 1);
  }
}

void setup() {
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Info);

  // setup output
  auto cfg = kit.defaultConfig(TX_MODE);
  cfg.sd_active = false;
  cfg.bits_per_sample = 16;
  kit.begin(cfg);

  // register callback
  a2dp_sink.set_stream_reader(read_data_stream, false);
  a2dp_sink.start("Rain Speaker");
}

void loop() {
  kit.processActions();
}
