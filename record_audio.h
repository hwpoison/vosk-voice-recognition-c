#ifndef RECORD_AUDIO_H
#define RECORD_AUDIO_H
#include <windows.h>
#include <mmsystem.h>

#define FORMAT_TAG              1
#define SAMPLERATE              44100
#define BITSPERSAMPLE           16
#define NCHANNELS               1
#define BLOCK_ALIGN             NCHANNELS * (BITSPERSAMPLE / 8)
#define AVG_BYTE_PER_SECOND     BLOCK_ALIGN * SAMPLERATE
#define BLOCK_SIZE              7000

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