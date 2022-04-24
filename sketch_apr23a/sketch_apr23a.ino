// Kit-bashed from examples
// basic-a2dp-audiokit
// 

#include "AudioTools.h"
#include "AudioLibs/AudioKit.h"
#include "AudioLibs/AudioA2DP.h"


BluetoothA2DPSink a2dp_sink;
AudioKitStream kit;

// Write data to AudioKit in callback
void read_data_stream(const uint8_t *data, uint32_t length) {
    kit.write(data, length);
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
  a2dp_sink.start("AudioKit");
}

void loop() {
  kit.processActions();
}
