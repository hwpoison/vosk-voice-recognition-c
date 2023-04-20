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
	int type;    	// PARTIAL_RESULT | ENTIRE_RESULT
	char *content; 
} recognition_result;


// Global audio block enqueue
static audio_block *queue_head;
static audio_block *queue_tail;
static int queue_size = 0;

// Global model and recognizer
VoskModel *gvosk_model;
VoskRecognizer *gvosk_recognizer;


// Initialize the gvosk_model and gvosk_recognizer
void initializeModelAndRecognizer();

// add audio block to queue
void enqueue_audio_block(audio_block *data);

// get audio block from queue
audio_block *dequeue_audio_block();

// Recognize audio block from audio_queue
void recognizeFromAudioBlockQueue(char *output_filename);

// Callback for read and add to queue a audio block from Microphone
void *getBlockFromMic();

// Analyze audio from a buffer
struct result_t *recognizeAudioBlock(char *data, int nlen);

// Voice to text from a .wav file
void recognizeWavFile(char *audio_filename, char *output_filename);

unsigned char *getFinalResult();

int checkFileExists(char *filename);

// write some text in a file
void writeToFile(char *filename, char *data, int nlen);

#endif
