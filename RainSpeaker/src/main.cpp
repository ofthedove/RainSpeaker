/* Kit-bashed from examples
 * basic-a2dp-audiokit
 * streams-a2dp-serial
 */ 

#define COPY_DELAY_ON_NODATA 0

#include "AudioTools.h"
#include "AudioLibs/AudioKit.h"
#include "AudioLibs/AudioA2DP.h"

A2DPStream a2dp_in = A2DPStream::instance(); // A2DP input - A2DPStream is a singleton!
AudioKitStream kit_out;
StreamCopy copier(kit_out, a2dp_in, 10); // copy in to out

void setup() {
  Serial.begin(115200);
  // AudioLogger::instance().begin(Serial, AudioLogger::Info);

  // setup output
  auto cfg = kit_out.defaultConfig(TX_MODE);
  cfg.sd_active = false;
  cfg.bits_per_sample = 16;
  kit_out.begin(cfg);

  // register callback
  auto cfg2 = a2dp_in.defaultConfig(RX_MODE);
  cfg2.name = "Rain Speaker";
  a2dp_in.begin(cfg2);  
}

void loop() {
  kit_out.processActions();
  copier.copy();
}
