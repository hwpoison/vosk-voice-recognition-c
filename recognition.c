#include "recognition.h"

#define true 1

void enqueue_audio_block(audio_block *data)
{
    if (queue_size == 0)
    {
        queue_head = data;
        queue_tail = data;
    }
    else
    {
        queue_tail->next = data;
        queue_tail = data;
    }
    queue_size++;
}

audio_block *dequeue_audio_block()
{
    if (queue_size == 0)
    {
        return NULL;
    }
    audio_block *data = queue_head;
    queue_head = queue_head->next;
    queue_size--;
    return data;
}

void *getBlockFromMic()
{
    audio_block *block = malloc(sizeof(audio_block));
    block->size = BLOCK_SIZE;
    block->data = malloc(sizeof(char) * block->size);
    while (true)
    {
        record(block->data, block->size);
        enqueue_audio_block(block);
    }
}

recognition_result *recognizeAudioBlock(VoskRecognizer *recognizer, char *data, int nlen)
{
    int final = vosk_recognizer_accept_waveform(recognizer, data, nlen);
    recognition_result *result = malloc(sizeof(struct result_t));
    if (final)
    {
        result->content = (char*)vosk_recognizer_result(recognizer);
	result->type = ENTIRE_RESULT;
    }
    else
    {
	result->content = (char*)vosk_recognizer_partial_result(recognizer);
	result->type = PARTIAL_RESULT;
    }
    return result;
}

void recognizeWavFile(VoskRecognizer *recognizer, char *filename)
{
    FILE *wavin;
    char buffer[BLOCK_SIZE];
    int nread;
    wavin = fopen(filename, "rb");
    fseek(wavin, 44, SEEK_SET); // offset 44 bytes to omits wav header
    while (!feof(wavin))
    {
        nread = fread(buffer, 1, BLOCK_SIZE, wavin);
        recognizeAudioBlock(recognizer, buffer, BLOCK_SIZE);
    }
    fclose(wavin);
}

unsigned char *getFinalResult(VoskRecognizer *recognizer)
{
    return (unsigned char *)vosk_recognizer_result(recognizer);
}

int checkFileExists(char *filename)
{
    FILE *file;
    file = fopen(filename, "r");
    if (file == NULL)
    {
        return 0;
    }
    else
    {
        fclose(file);
        return 1;
    }
}

void writeToFile(char *filename, char *data, int nlen)
{
    FILE *file;
    file = fopen(filename, "a");
    if (file == NULL)
    {
        fprintf(stderr, "[x] Error opening file\n");
    }
    else
    {
        fwrite(data, sizeof(char), nlen, file);
        fclose(file);
    }
}
