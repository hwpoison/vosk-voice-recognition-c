#ifndef WAVUTILS_H
#define WAVUTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "record_audio.h"

// Conversion from Big Endian to Little Endian
uint32_t *to_little_endian(uint32_t x);

// Play a wav file
void playWavFile(char *filename);
 
// Play wavform raw audio data
void playBuffer(char *rawdata, int nlen);

// Record audio from mic and saves in a .wav file
void recordAndSaveWav(char *filename, int secs);

// Generate a playable .wav file from a raw wavform
void generateWav(char *filename, char *rawdata, int size);

#endif