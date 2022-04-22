#ifndef WAVUTILS_H
#define WAVUTILS_H
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "record_audio.h"

// Conversion from Big Endian to Little Endian
uint32_t *to_little_endian(uint32_t x);

// Play a wav file
void PlayWavFile(char *filename);
 
// Play wavform raw audio data
void PlayBuffer(char *rawdata, int nlen);

// Record audio from mic and saves in a .wav file
void RecordAndSaveWav(char *filename, int secs);

// Generate a playable .wav file from a raw wavform
void GenerateWav(char *filename, char *rawdata, int size);

#endif