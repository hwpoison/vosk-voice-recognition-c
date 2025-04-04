#ifndef RECORD_AUDIO_H
#define RECORD_AUDIO_H
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>

#define FORMAT_TAG              1                                   // PCM
#define SAMPLERATE              16000                               // in hertz
#define BITSPERSAMPLE           16                                  // 16-bit
#define NCHANNELS               1                                   // 1 Channel mono audio
#define BLOCK_ALIGN             NCHANNELS * (BITSPERSAMPLE / 8) 
#define AVG_BYTE_PER_SECOND     BLOCK_ALIGN * SAMPLERATE
#define BLOCK_SIZE              8000                               //  amount of bytes per audio chunk

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

// for list all available devices
void list_output_devices();

void list_input_devices();

// initialize the audio i/o devices
int init_input_device(int id);

int init_output_device(int id);

// play or record
int record(char *data, int length);

int play(char *data, int length);

// close all
void close_audio();

#endif
