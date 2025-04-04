### Vosk C Implementation 🎙️
-------------
This is a implementation in C using Vosk offline speech recognition lib for Windows

#### Requirements
- libvosk.dll and pthread dependences in windows. Download from https://github.com/alphacep/vosk-api/releases/tag/v0.3.427
- Get a voice model from https://alphacephei.com/vosk/models (https://alphacephei.com/vosk/models/vosk-model-small-es-0.3.zip for spanish lang)

#### How to use
You can run the Vosk_recognizer binary with next args

| Command argument| Description|
| ------------ | ------------ |
|`--mic` |To use the microphone|
|`--file <filename>.wav` |To read a .wav file|
|`--device-id <id>`  |To use a determinated device id|
|`--list-devices`| To view all available audio I/O devices|

#### About supported audio format
Vosk by default expect a wav file with a sample rate of 16kHz (16000Hz) an a bitsample of 16bit PCM. So the mic needs also to be switched to 16khz/16bit.
https://alphacephei.com/vosk/install#:~:text=When%20using%20your%20own%20audio%20file%20make%20sure%20it%20has%20the%20correct%20format%20%2D%20PCM%2016khz%2016bit%20mono.%20Otherwise%2C%20if%20you%20have%20ffmpeg%20installed%2C%20you%20can%20use%20test_ffmpeg.py%2C%20which%20does%20the%20conversion%20for%20you.

#### How to build:
Put all the dependences under source code dir then run

`make all`


