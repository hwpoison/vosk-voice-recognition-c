#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include "recognition.h"
#include "record_audio.h"

#define true 1
#define NUM_THREADS 2

int main(int argc, char **argv)
{
    // get first argument
    if (argc < 2)
    {
        printf("Usage: %s.exe --file <filename>\n\t\t\t  --mic", argv[0]);
        return 1;
    }

    char filename[100];
    queue_size = 0;
    pthread_t thread_pool[NUM_THREADS];

    printf("[+] Initializing model...\n");
    VoskModel *model = vosk_model_new("model");
    VoskRecognizer *recognizer = vosk_recognizer_new(model, SAMPLERATE);

    char *option = argv[1];
    if (!strcmp(option, "--mic"))
    {
        printf("[+] Initializing microphone...\n");
        int rc = pthread_create(&thread_pool[1], NULL, getBlockFromMic, NULL);
        while (true)
        {
            struct audio_block *block = dequeue_audio_block();
            if (block != NULL)
            {
                recognizeAudioBlock(recognizer, block->data, block->size);
            }
        }
        printf("[+] Final result %s\n", getFinalResult(recognizer));
        pthread_exit(NULL);
    }
    else if (!strcmp(option, "--file") && argc == 3)
    {
        strcpy(filename, argv[2]);
        if (checkFileExists(filename))
        {
            recognizeWavFile(recognizer, filename);
            char *result = getFinalResult(recognizer);
            printf("[+] Final result: %s\n", result);
            char result_file[100];
            strcpy(result_file, filename);
            strcat(result_file, ".transcription.txt");
            writeToFile(result_file, result, strlen(result));
        }
        else
        {
            printf("[x] File not found!\n");
        }
    }
    else
    {
        printf("[x] Invalid argument\n");
        return 1;
    }

    printf("Releasing memory...\n");
    vosk_recognizer_free(recognizer);
    vosk_model_free(model);
    return 0;
}
