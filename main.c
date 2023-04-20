#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <windows.h>
#include <strings.h>

#include "recognition.h"
#include "record_audio.h"

#define true 1
#define NUM_THREADS 1
#define MAX_FILENAME_LENGTH 100

int main(int argc, char **argv)
{
    char input_filename[MAX_FILENAME_LENGTH];
    char *file_option_str = "--file";
    char *mic_option_str = "--mic";
    
    int micThread;
    queue_size = 0;
    pthread_t thread_pool[NUM_THREADS];

    // Get first argument
    if (argc < 2)
    {
        printf(":=VoskC=: by hwpoison\n\n Usage: %s %s <filename>\n\t\t\t    %s\n", argv[0], file_option_str, mic_option_str);
        return 1;
    }

    // Initialize the mod and recognizer
    printf("[+] Initializing model and the recognizer.\n");
    initializeModelAndRecognizer();

    // Check arguments options
    char *option = argv[1];
    if (option != NULL && !strcmp(option, mic_option_str))
    {
	// MIC Transcription
        printf("[+] Initializing microphone mode...\n");
    	printf("[+] Starting recording thread...\n");
	// A thread that capture all audio blocks from mic
	// that are enqueue to the audio_queue
        micThread = pthread_create(&thread_pool[1], NULL, getBlockFromMic, NULL);
	// Then are dequeue and analize to recognize the content
	recognizeFromAudioBlockQueue( "mic_transcription.txt");
        pthread_exit(NULL); // kill threads
    }
    else if (!strcmp(option, file_option_str) && argc == 3)
    {
        // WAV Transcription
        strcpy(input_filename, argv[2]);
        if (checkFileExists(input_filename))
        {
	    printf("[+] Recognizing audio file...\n");
            char transcription_filename[100]; // filename + suffix
            strcpy(transcription_filename, input_filename);
            strcat(transcription_filename, ".transcription.txt");
            recognizeWavFile(input_filename, transcription_filename);
        }
        else
        {
            fprintf(stderr, "[x] File not found!\n");
            return 1;
        }
    }
    else
    {
	printf("%d\n", argc);
        fprintf(stderr, "[x] Invalid argument\n");
        return 1;
    }
    vosk_recognizer_free(gvosk_recognizer);
    vosk_model_free(gvosk_model);
    printf("[+] Exited.\n");
    return 0;
}
