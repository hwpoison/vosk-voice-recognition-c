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

struct VoskModel *gvosk_model = NULL;
struct VoskRecognizer *gvosk_recognizer = NULL;


// aux function to extract args values
const char *get_arg_value(int argc, char **argv, const char *target_arg){
    for(int arg_idx = 0; arg_idx < argc; arg_idx++){
        if(!strcmp(argv[arg_idx], target_arg)) // <arg> <value> 
            return argv[arg_idx+1]==NULL?"":argv[arg_idx+1];
    }
    return NULL;
}

int main(int argc, char **argv)
{
    const char *input_arg = NULL;

    char input_filename[MAX_FILENAME_LENGTH];
    char *file_option_str = "--file";
    char *mic_option_str = "--mic";
    int input_device_id = 0;

    int micThread;
    queue_size = 0;
    pthread_t thread_pool[NUM_THREADS];

    // first check if there is some arguments
    if (argc < 2)
    {
        printf(":=VoskC=: by hwpoison\n\n Usage: %s %s <filename>\n\t\t\t    %s\n", argv[0], file_option_str, mic_option_str);
        return 1;
    }

    // just list the devices
    if ((input_arg = get_arg_value(argc, argv, "--list-devices")) != NULL){
        list_input_devices();
        printf("\n");
        list_output_devices();
        return 0;
    }
    
    atexit(recognizer_exit);

    
    if((input_arg = get_arg_value(argc, argv, "--device-id")) != NULL){
        printf("[+] Input device selectd: %s\n", input_arg);
        input_device_id = atoi(input_arg);
    }
        
    if ((input_arg = get_arg_value(argc, argv, "--mic")) != NULL){
        initializeModelAndRecognizer();
        // MIC Transcription
        printf("[+] Initializing microphone mode...\n");
        // Initialize mic in out device
        init_input_device(input_device_id);
        printf("[+] Starting recording thread...\n");
       // A thread that capture all audio blocks from mic
       // that are enqueue to the audio_queue
       micThread = pthread_create(&thread_pool[1], NULL, getBlockFromMic, NULL);
       // Then are dequeue and analize to recognize the content
       recognizeFromAudioBlockQueue( "mic_transcription.txt");
       // then kill threads when finished
       pthread_exit(NULL); 
    }
    else if ((input_arg = get_arg_value(argc, argv, "--file")) != NULL){
        initializeModelAndRecognizer();
        // WAV Transcription
        strcpy(input_filename, input_arg);
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
    }else{
        fprintf(stderr, "[x] Invalid argument\n");
    }

    printf("[+] Exiting.\n");
    return 0;
}


