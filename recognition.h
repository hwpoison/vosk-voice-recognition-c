#ifndef RECOGNITION_H
#define RECOGNITION_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "vosk_api.h"
#include "wavutils.h"
#include "record_audio.h"

#define PARTIAL_RESULT 10
#define ENTIRE_RESULT  11

typedef struct audio_block_t {
    char *data;
    int size;
    struct audio_block_t *next;
} audio_block;

typedef struct result_t {
	int type;
	char *content;
} recognition_result;


// Global audio block enqueue
static audio_block *queue_head;
static audio_block *queue_tail;
static int queue_size = 0;

// add audio block to queue
void enqueue_audio_block(audio_block *data);

// get audio block from queue
audio_block *dequeue_audio_block();

// Callback for read and add to queue a audio block from Microphone
void *getBlockFromMic();

// Analyze audio from a buffer
struct result_t *recognizeAudioBlock(VoskRecognizer *recognizer, char *data, int nlen);

// Voice to text from a .wav file
void recognizeWavFile(VoskRecognizer *recognizer, char *filename);

unsigned char *getFinalResult(VoskRecognizer *recognizer);

int checkFileExists(char *filename);

// write some text in a file
void writeToFile(char *filename, char *data, int nlen);

#endif
