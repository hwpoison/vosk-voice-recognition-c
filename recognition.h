#ifndef RECOGNITION_H
#define RECOGNITION_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vosk_api.h"
#include "wavutils.h"

struct audio_block {
    char *data;
    int size;
    struct audio_block *next;
};

// Global audio block enqueue
static struct audio_block *queue_head;
static struct audio_block *queue_tail;
static int queue_size = 0;

// add audio block to queue
void enqueue_audio_block(struct audio_block *data);

// get audio block from queue
struct audio_block *dequeue_audio_block();

// Callback for read and add to queue a audio block from Microphone
void *getBlockFromMic();

// Callback for read and add to queue a audio block from Microphone
void *getBlockFromFile(void *filename);

// Analyze audio from a buffer
void recognizeAudioBlock(VoskRecognizer *recognizer, char *data, int nlen);

// Voice to text from a .wav file
void recognizeWavFile(VoskRecognizer *recognizer, char *filename);

#endif