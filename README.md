# Rain Speaker
A successor to [`ofthedove/RainBox`](https://github.com/ofthedove/rainbox) using a LyraT dev board.

## Project Structure
- Audio
  - This folder contains the original and edited rain noise files. These are loaded to the SD card
- RainSpeaker
  - This is the PlatformIO project for the LyraT in the speaker itself
- RainSpeakerVolumeKnob
  - A separate PlatformIO project for the volume remote, which is just an ESP32 with an encoder
