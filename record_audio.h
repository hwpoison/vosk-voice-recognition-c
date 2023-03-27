#ifndef RECORD_AUDIO_H
#define RECORD_AUDIO_H
#include <windows.h>
#include <mmsystem.h>

#define DEVICE_ID 		0
#define FORMAT_TAG              1                                   // PCM
#define SAMPLERATE              16000                               // 16000hz
#define BITSPERSAMPLE           16                                  // 16-bit
#define NCHANNELS               1                                   // 1 Channel mono audio
#define BLOCK_ALIGN             NCHANNELS * (BITSPERSAMPLE / 8) 
#define AVG_BYTE_PER_SECOND     BLOCK_ALIGN * SAMPLERATE
#define BLOCK_SIZE              4000                                //  amount of bytes per audio chunk

struct WAV_HEADER {
    int     formatTag;
    int     sampleRate;
    int     bitsPerSample;
    int     nChannels;
    int     blockAlign;
    int     avgBytesPerSecond;
    int     blockSize;
};

static struct WAV_HEADER wav = {
    FORMAT_TAG,
    SAMPLERATE,
    BITSPERSAMPLE,
    NCHANNELS,
    BLOCK_ALIGN,
    AVG_BYTE_PER_SECOND,
    BLOCK_SIZE
};

int record(char *data, int length);

int play(char *data, int length);


#endif
