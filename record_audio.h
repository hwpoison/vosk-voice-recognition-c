#ifndef RECORD_AUDIO_H
#define RECORD_AUDIO_H
#include <windows.h>
#include <mmsystem.h>

#define FORMAT_TAG          1 // PCM
#define SAMPLERATE          16000 // Sample frequency per second
#define BITSPERSAMPLE       16 // 
#define NCHANNELS           1 // 1 - Mono or 2 - Stereo
#define BLOCK_ALIGN         NCHANNELS * (BITSPERSAMPLE / 8)
#define AVG_BYTE_PER_SECOND BLOCK_ALIGN * SAMPLERATE
#define BLOCK_SIZE 6000

int startRecord(char *data, int length);
int startPlayback(char *data, int length);

#endif