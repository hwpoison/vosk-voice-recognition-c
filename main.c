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
    char filename[MAX_FILENAME_LENGTH];
    char *file_option_str = "--file";
    char *mic_option_str = "--mic";
    
    int micThread;
    queue_size = 0;
    pthread_t thread_pool[NUM_THREADS];

    // get first argument
    if (argc < 2)
    {
        printf(":=VoskC=: by hwpoison\n\n Usage: %s %s <filename>\n\t\t\t    %s\n", argv[0], file_option_str, mic_option_str);
        return 1;
    }

    // Instance and initialize the mod and recognizer
    printf("[+] Initializing model and the recognizer.\n");
    VoskModel *model = vosk_model_new("model");
    if(model == NULL){
	    fprintf(stderr, "[x] ERROR TO LOAD MODEL, CHECK IF EXISTS IN THE FOLDER\n");
	    exit(-1);
    }
    VoskRecognizer *recognizer = vosk_recognizer_new(model, wav.sampleRate);

    // Check arguments options
    char *option = argv[1];
    if (option != NULL && !strcmp(option, mic_option_str))
    {
        printf("[+] Initializing microphone mode...\n");
    	printf("[+] Starting recording thread...\n");
        micThread = pthread_create(&thread_pool[1], NULL, getBlockFromMic, NULL);
        audio_block *audio_queue_head;
	recognition_result *result = malloc(sizeof(struct result_t*));

        while (true)
        {
	    audio_queue_head = dequeue_audio_block();
	    if(audio_queue_head != NULL){
            	result = recognizeAudioBlock(recognizer, audio_queue_head->data, audio_queue_head->size);
		printf("%s\n", result->content);
		
		// when recognition is finished and it has content, reset recognizer
	        if(result->type == ENTIRE_RESULT && !strstr(result->content, "\"\"")){
			printf("%s\n", result->content);
			writeToFile("mic_transcription.txt", result->content, strlen(result->content));
			vosk_recognizer_free(recognizer);
			recognizer = vosk_recognizer_new(model, wav.sampleRate);
		}
	    }
        }
        pthread_exit(NULL); // kill threads
    }
    else if (!strcmp(option, file_option_str) && argc == 3)
    {
        strcpy(filename, argv[2]);
        if (checkFileExists(filename))
        {
	    printf("[+] Recognizing audio file...");
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
    vosk_recognizer_free(recognizer);
    vosk_model_free(model);
    printf("[+] Exited.\n");
    return 0;
}
