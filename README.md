# Rain Speaker
A successor to [`ofthedove/RainBox`](https://github.com/ofthedove/rainbox) using a LyraT dev board.

## Project Structure
- Audio
  - This folder contains the original and edited rain noise files. These are loaded to the SD card
- RainSpeaker_Hass
  - This is the new code for the speaker itself, using the new library and relying on Home Assistant for volume control.
  - It has targets for the LyraT in the speaker itself, and for the AudioKit board I got for development.
- RainSpeaker [DEPRECATED]
  - This is the PlatformIO project for the LyraT in the speaker itself
  - This is deprecated because I couldn't get it to build with Bluetooth working, so I disabled bluetooth so I could build for debugging purposes. I haven't made an effort to get it building with bluetooth and don't intend to, I won't support it going forward
- RainSpeakerVolumeKnob [DEPRECATED]
  - A separate PlatformIO project for the volume remote, which is just an ESP32 with an encoder
  - Since RainSpeaker is deprecated, I won't support this anymore either.
